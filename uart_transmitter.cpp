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

    uart_transmitterData.transmit_idx = 0;
    uart_transmitterData.transmit_str[0] = 0x80u;
    uart_transmitterData.transmit_str[1] = 0x08u;
    uart_transmitterData.transmit_str[2] = 0x13u;
    uart_transmitterData.transmit_str[3] = 0x55u;
    memcpy(uart_transmitterData.transmit_str + 4, &receivedMessage,
           sizeof(receivedMessage));
    uart_transmitterData.transmit_size = sizeof(receivedMessage) + 4;
    uart_transmitterData.state = UART_TRANSMITTER_STATE_SEND;
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
