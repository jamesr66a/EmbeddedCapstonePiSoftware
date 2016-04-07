#include "debug.h"
#include "debuginfo.h"
#include "generated/DebugInfo.pbo.h"
#include "motor_model_public.h"
#include "pid_model_public.h"
#include "rssi_model_public.h"
#include "sensors_model_public.h"
#include "serial.h"
#include "uart_receiver.h"
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

void UART_RECEIVER_Initialize(int fd) {
  uart_receiverData.state = UART_RECEIVER_STATE_INIT;
  // Initialize callback vector
  uart_receiverData.uart_receiver_callbacks_idx = 0;

  uart_receiverData.receive_fd = fd;
}

// Consume a message from the receive queue and unpack it as a char.
// Blocks if there is no message available.
static uint8_t receive_and_unpack() {
  uint8_t data;

  if (EOF != read(uart_receiverData.receive_fd, &data, 1)) {
    return data;
  } else {
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
      uart_receiverData.receive_buf_idx = 0;
      uart_receiverData.state = UART_RECEIVER_STATE_RECEIVE;
    } else {
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
    }
  } break;

  // We've made it through the frame delimiter sequence. Receive a full message.
  case UART_RECEIVER_STATE_RECEIVE: {
    // Populate the current byte in our receive buffer.
    uart_receiverData.receive_buf[uart_receiverData.receive_buf_idx++] =
        receive_and_unpack();

    // If we've received the correct number of bytes for a message, parse the
    // message.
    if (uart_receiverData.receive_buf_idx == sizeof(DebugInfo)) {
      // Proto object we're parsing into
      DebugInfo received_obj;

      // Sequence number
      uint32_t seq;
      static uint32_t seq_expected;

      // Parse from bytes. Also does a hash integrity check
      if (!DebugInfo_from_bytes(&received_obj, uart_receiverData.receive_buf,
                                &seq)) {
        // If we fail our integrity check, fail.
        // TODO: notify
        std::cerr << "Hash check failed" << std::endl;
        uart_receiverData.state = UART_RECEIVER_FRAME_START_1;
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

      // Switch back to receiving the first byte of the frame delimiter word.
      uart_receiverData.state = UART_RECEIVER_FRAME_START_1;

      /*std::cout << "DebugInfo Received: " <<
         DebugInfo_identifier(&received_obj)
                << " " << DebugInfo_debugID(&received_obj) << " "
                << DebugInfo_data(&received_obj) << " sequence number: " << seq
                << "\n";*/
      sendToWebserverModelQueue(&received_obj);

      if (DebugInfo_identifier(&received_obj) == SENSOR1_IDENTIFIER) {
        sendToSensorsModelQueue(&received_obj);
      }
      if (DebugInfo_identifier(&received_obj) == RSSI_COLLECTOR_IDENTIFIER) {
        sendToRSSIModelQueue(&received_obj);
      }
      if (DebugInfo_identifier(&received_obj) == PID_IDENTIFIER) {
        sendToPIDModelQueue(&received_obj);
      }
      if (DebugInfo_identifier(&received_obj) == MOTOR1_IDENTIFIER) {
        sendToMOTORModelQueue(&received_obj);
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
