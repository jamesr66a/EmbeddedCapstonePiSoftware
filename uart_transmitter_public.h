#ifndef _UART_TRANSMITTER_PUBLIC_H_
#define _UART_TRANSMITTER_PUBLIC_H_

#include "generated/WiFlyStringWireFormat.pbo.h"

typedef enum { STRING_WIRE_FORMAT } UART_TRANSMITTER_VARIANT_TYPE;

struct UART_TRANSMITTER_VARIANT {
  union {
    WiFlyStringWireFormat debug_info;
  } data;
  UART_TRANSMITTER_VARIANT_TYPE type;
};

bool sendToUartQueue(struct UART_TRANSMITTER_VARIANT *var);

#endif /* _UART_TRANSMITTER_PUBLIC_H_ */
