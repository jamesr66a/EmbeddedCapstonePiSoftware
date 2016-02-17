#ifndef _UART_RECEIVER_PUBLIC_H_
#define _UART_RECEIVER_PUBLIC_H_

#include "generated/UartRxData.pbo.h"
#include "generated/WiFlyStringWireFormat.pbo.h"
#include "SafeQueue.h"

typedef enum { UART_RX_DATA, STRING } UART_RECEIVER_VARIANT_TYPE;

struct UART_RECEIVER_VARIANT {
  union {
    UartRxData rx_data;
    WiFlyStringWireFormat string;
  } data;
  UART_RECEIVER_VARIANT_TYPE type;
};

typedef int (*uart_receiver_callback_t)(struct UART_RECEIVER_VARIANT *);

int registerUartReceiverCallback(uart_receiver_callback_t callback);

#endif /* _UART_RECEIVER_PUBLIC_H_ */
