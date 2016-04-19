#ifndef _UART_RECEIVER_H_
#define _UART_RECEIVER_H_

#include "generated/UartRxData.pbo.h"
#include "generated/WiFlyStringWireFormat.pbo.h"
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "uart_receiver_public.h"

typedef enum {
  UART_RECEIVER_STATE_INIT = 0,
  UART_RECEIVER_FRAME_START_1,
  UART_RECEIVER_FRAME_START_2,
  UART_RECEIVER_FRAME_START_3,
  UART_RECEIVER_FRAME_START_4,
  UART_RECEIVER_FRAME_SWITCH,
  UART_RECEIVER_STATE_RECEIVE,
} UART_RECEIVER_STATES;

#define UART_RECEIVER_CALLBACKS_VECTOR_SIZE (10)
#define UART_RECEIVER_QUEUE_SIZE (10)

typedef struct {
  UART_RECEIVER_STATES state;

  // Callbacks vector members
  size_t uart_receiver_callbacks_idx;
  uart_receiver_callback_t callbacks[UART_RECEIVER_CALLBACKS_VECTOR_SIZE];

  // Receive file descriptor
  int receive_fd;

  // Receive buffer members
  size_t receive_buf_idx;
  char receive_buf[sizeof(struct UART_RECEIVER_VARIANT)+5];

  size_t rx_size;
  uint8_t msg_type;
} UART_RECEIVER_DATA;

void UART_RECEIVER_Initialize(int fd);

void UART_RECEIVER_Tasks(void);

void uart_receiver_thread_run();

#endif /* _UART_RECEIVER_H_ */
