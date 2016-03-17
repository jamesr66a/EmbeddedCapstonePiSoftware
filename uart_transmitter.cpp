#include <string.h>
#include <unistd.h>

#include "uart_transmitter.h"

#include "debug.h"
#include "uart_transmitter.h"

#define QSIZE 10
UART_TRANSMITTER_DATA uart_transmitterData;

// Public functions

bool sendToUartQueue(struct UART_TRANSMITTER_VARIANT *var) {
  uart_transmitterData.rx_queue.enqueue(*var);
  return true;
}

// Internal functions

void UART_TRANSMITTER_Initialize(int fd) {
  uart_transmitterData.state = UART_TRANSMITTER_STATE_INIT;

  uart_transmitterData.transmit_fd = fd;
}

void UART_TRANSMITTER_Tasks(void) {

  /* Check the application's current state. */
  switch (uart_transmitterData.state) {
  /* Application's initial state. */
  case UART_TRANSMITTER_STATE_INIT: {
    uart_transmitterData.state = UART_TRANSMITTER_STATE_RECEIVE;
    break;
  }
  case UART_TRANSMITTER_STATE_RECEIVE: {
    struct UART_TRANSMITTER_VARIANT receivedMessage;
    // Receive a message on the created queue.
    receivedMessage = uart_transmitterData.rx_queue.dequeue();
    switch (receivedMessage.type) {
    case STRING_WIRE_FORMAT: {
      // receivedMessage now points to the struct
      // UART_TRANSMITTER_VARIANT variable posted by vATask.
      WiFlyStringWireFormat deserialized_message;
      static uint32_t seq_expected;
      uint32_t seq;
      if (!WiFlyStringWireFormat_from_bytes(
              &deserialized_message, (char *)&receivedMessage.data.debug_info,
              &seq)) {
        // TODO: handle gracefully
        errorCheck();
      }
      if (seq != seq_expected) {
        // TODO: notify
      }
      seq_expected++;

      static uint32_t stringwireformat_seq;
      WiFlyStringWireFormat_to_bytes(&deserialized_message,
                                     uart_transmitterData.transmit_str,
                                     stringwireformat_seq);
      uart_transmitterData.transmit_idx = 0;
      uart_transmitterData.transmit_str[0] = 0x13;
      uart_transmitterData.transmit_str[1] = 0x37;
      uart_transmitterData.transmit_str[2] = 0x80;
      uart_transmitterData.transmit_str[3] = 0x08;
      memcpy(uart_transmitterData.transmit_str + 4, &deserialized_message,
             sizeof(deserialized_message));
      uart_transmitterData.transmit_size = sizeof(deserialized_message) + 4;

      uart_transmitterData.state = UART_TRANSMITTER_STATE_SEND;
    default: { break; }
    }
    }
  } break;

  case UART_TRANSMITTER_STATE_SEND: {
    uart_transmitterData.transmit_idx = write(
        uart_transmitterData.transmit_fd, uart_transmitterData.transmit_str,
        uart_transmitterData.transmit_size - uart_transmitterData.transmit_idx);
    if (uart_transmitterData.transmit_idx == -1u) {
      perror("Transmit error");
      exit(-1);
    }
    if (uart_transmitterData.transmit_idx ==
        uart_transmitterData.transmit_size) {
      uart_transmitterData.state = UART_TRANSMITTER_STATE_RECEIVE;
    }
  } break;

  default: {
    errorCheck();
    break;
  }
  }
}

void uart_transmitter_thread_run() {
  while (true) {
    UART_TRANSMITTER_Tasks();
  }
}
