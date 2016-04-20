#include "debug.h"
#include "debuginfo.h"
#include "encoders_model_public.h"
#include "errorcheck_model_public.h"
#include "generated/DebugInfo.pbo.h"
#include "motor_model_public.h"
#include "pose_model_public.h"
#include "pid_model_public.h"
#include "rssi_model_public.h"
#include "sensors_model_public.h"
#include "serial.h"
#include "uart_receiver.h"
#include "warning_model_public.h"
#include "webserver_model.h"

#include <iostream>
#include <unistd.h>

UART_RECEIVER_DATA uart_receiverData;

// Byte sequence that indicates the start of a frame

static const unsigned char frameSequence[4] = { 0x13, 0x37, 0x80, 0x08 };

// Internal functions

static void sendMessageToCallbacks(struct UART_RECEIVER_VARIANT *info) {
  size_t i;
  for (i = 0; i < uart_receiverData.uart_receiver_callbacks_idx; i++) {
    uart_receiverData.callbacks[i](info);
  }
}

void UART_RECEIVER_Initialize(int fd, std::string out_csv_file) {
  uart_receiverData.state = UART_RECEIVER_STATE_INIT;
  // Initialize callback vector
  uart_receiverData.uart_receiver_callbacks_idx = 0;

  uart_receiverData.receive_fd = fd;
  uart_receiverData.rx_size = 0;
  uart_receiverData.msg_type = 0;

  uart_receiverData.out_csv.open(out_csv_file, std::ios_base::app);
}

// Consume a message from the receive queue and unpack it as a char.
// Blocks if there is no message available.
static uint8_t receive_and_unpack() {
  uint8_t data;

  if (EOF != read(uart_receiverData.receive_fd, &data, 1)) {
    return data;
  } else {
    std::cout << "EOF\n";
    errorCheck();
    return -1;
  }
}

void UART_RECEIVER_Tasks(void) {
  switch (uart_receiverData.state) {
  case UART_RECEIVER_STATE_INIT: {
    uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
    break;
  }

  // Receive the first byte of the frame delimiter word. If the received byte
  // does not match, stay in this state.
  case UART_RECEIVER_FRAME_START_1: {
    if (receive_and_unpack() == frameSequence[0]) {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_2;
    }
  } break;

  // Receive the second byte of the frame delimiter word. If the received byte
  // does not match, fall back to state 1
  case UART_RECEIVER_FRAME_START_2: {
    if (receive_and_unpack() == frameSequence[1]) {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_3;
    } else {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
    }
  } break;

  // Receive the third byte of the frame delimiter word. If the received byte
  // does not match, fall back to state 1
  case UART_RECEIVER_FRAME_START_3: {
    if (receive_and_unpack() == frameSequence[2]) {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_4;
    } else {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
    }
  } break;

  // Receive the fourth byte of the frame delimiter word. If the received byte
  // does not match, fall back to state 1
  case UART_RECEIVER_FRAME_START_4: {
    if (receive_and_unpack() == frameSequence[3]) {
      // Set our receive index to 0 so we receive a brand new message.
      uart_receiverData.state = UART_RECEIVER_FRAME_SWITCH;
    } else {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
    }
  } break;

  case UART_RECEIVER_FRAME_SWITCH: {
    uint8_t rxed_byte = receive_and_unpack();
    switch (rxed_byte) {
    case 0:
      uart_receiverData.msg_type = 0;
      uart_receiverData.rx_size = sizeof(struct UART_RECEIVER_VARIANT);
      break;
    case 1:
      uart_receiverData.msg_type = 1;
      uart_receiverData.rx_size = sizeof(struct UART_RECEIVER_VARIANT_WIRE);
      break;
    default:
      std::cout << "rxed_byte: " << rxed_byte << std::endl;
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
      return;
    }
    uart_receiverData.receive_buf_idx = 0;
    uart_receiverData.state = UART_RECEIVER_STATE_RECEIVE;
  } break;

  // We've made it through the frame delimiter sequence. Receive a full message.
  case UART_RECEIVER_STATE_RECEIVE: {
    // Populate the current byte in our receive buffer.
    uart_receiverData.receive_buf[uart_receiverData.receive_buf_idx++] =
        receive_and_unpack();

    // If we've received the correct number of bytes for a message, parse the
    // message.
    if (uart_receiverData.receive_buf_idx == uart_receiverData.rx_size) {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
      if (uart_receiverData.msg_type == 0) {
        struct UART_RECEIVER_VARIANT var;
        memcpy(&var, uart_receiverData.receive_buf, sizeof(var));
        uint32_t a;
        if (RSSIData_from_bytes(&var.data.rssi_pair.rssi,
                                (char *)&var.data.rssi_pair.rssi, &a)) {
          char buf[100];
          RSSIData_bssid(&var.data.rssi_pair.rssi, buf, sizeof(buf));
          std::cout << RoverPose_xPosition(&var.data.rssi_pair.pose) << " "
                    << RoverPose_yPosition(&var.data.rssi_pair.pose) << " "
                    << RoverPose_yaw(&var.data.rssi_pair.pose) << "\n" << buf
                    << " " << RSSIData_rssi(&var.data.rssi_pair.rssi) << "\n";
          uart_receiverData.out_csv
              << RSSIData_frameNum(&var.data.rssi_pair.rssi) << "," << buf
              << "," << RSSIData_rssi(&var.data.rssi_pair.rssi) << ","
              << RoverPose_xPosition(&var.data.rssi_pair.pose) << ","
              << RoverPose_yPosition(&var.data.rssi_pair.pose) << ","
              << RoverPose_yaw(&var.data.rssi_pair.pose) << "\n" << std::flush;
        }
      } else {
        struct UART_RECEIVER_VARIANT_WIRE var;
        memcpy(&var, uart_receiverData.receive_buf,
               sizeof(struct UART_RECEIVER_VARIANT_WIRE));
        switch (var.type) {
        case DEBUG_INFO: {
          // Proto object we're parsing into
          DebugInfo received_obj;

          // Sequence number
          uint32_t seq;
          static uint32_t seq_expected;

          // Parse from bytes. Also does a hash integrity check
          if (!DebugInfo_from_bytes(&received_obj, (char *)&var.data.debug_info,
                                    &seq)) {
            // If we fail our integrity check, fail.
            // TODO: notify
            std::cerr << "Hash check failed" << std::endl;
            seq_expected++;
            return;
          }

          if (seq != seq_expected) {
            std::cerr << "Missing sequence number. Expected: " << seq_expected
                      << " Got: " << seq << "\n";
            seq_expected = seq + 1;
          } else {
            seq_expected++;
          }

          if (DebugInfo_identifier(&received_obj) != ERRORCHECK_IDENTIFIER &&
              DebugInfo_identifier(&received_obj) != WARNING_IDENTIFIER) {
            sendToWebserverModelQueue(&received_obj);
          }

          if (DebugInfo_identifier(&received_obj) == SENSOR1_IDENTIFIER) {
            sendToSensorsModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) ==
              RSSI_COLLECTOR_IDENTIFIER) {
            sendToRSSIModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) == PID_IDENTIFIER) {
            sendToPIDModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) == MOTOR1_IDENTIFIER) {
            sendToMOTORModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) == ERRORCHECK_IDENTIFIER) {
            sendToErrorCheckModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) == WARNING_IDENTIFIER) {
            sendToWarningModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) == ENCODER1_IDENTIFIER) {
            sendToEncodersModelQueue(&received_obj);
          }
          if (DebugInfo_identifier(&received_obj) == POSE_IDENTIFIER) {
            sendToPoseModelQueue(&received_obj);
          }
        } // case DEBUG_INFO
        case TEST_CHAR: {
        } break;
        } // switch (var.type)
      }
    }
  } break;

  default: { break; }
  }
}

void uart_receiver_thread_run() {
  while (true) {
    UART_RECEIVER_Tasks();
  }
}
