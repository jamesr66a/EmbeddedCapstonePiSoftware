#ifndef _UART_TRANSMITTER_H_
#define _UART_TRANSMITTER_H_

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "SafeQueue.h"
#include "uart_transmitter_public.h"

// Application states
typedef enum {
  /* Application's state machine's initial state. */
  UART_TRANSMITTER_STATE_INIT = 0,
  UART_TRANSMITTER_STATE_SEND = 1,
  UART_TRANSMITTER_STATE_RECEIVE = 2,
} UART_TRANSMITTER_STATES;

// Application Data
typedef struct {
  /* The application's current state */
  UART_TRANSMITTER_STATES state;
  SafeQueue<struct UART_TRANSMITTER_VARIANT> rx_queue;

  char transmit_str[sizeof(struct UART_TRANSMITTER_VARIANT) + 4];
  size_t transmit_idx;
  size_t transmit_size;

  int transmit_fd;
} UART_TRANSMITTER_DATA;

void UART_TRANSMITTER_Initialize(int fd);

void UART_TRANSMITTER_Tasks(void);

void uart_transmitter_thread_run();

#endif /* _UART_TRANSMITTER_H_ */
