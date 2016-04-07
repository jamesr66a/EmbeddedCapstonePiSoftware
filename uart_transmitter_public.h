#ifndef UART_TRANSMITTER_PUBLIC_H
#define UART_TRANSMITTER_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#include "generated/MotorCommand.pbo.h"
#include "generated/UartRxData.pbo.h"
#include "generated/WiFlyStringWireFormat.pbo.h"

typedef enum {
  UART_RX_DATA_TX,
  STRING_TX,
  MOTOR_MESSAGE_TX
} UART_TRANSMITTER_VARIANT_TYPE;

struct __attribute__((packed)) UART_TRANSMITTER_VARIANT {
  union {
    UartRxData rx_data;
    WiFlyStringWireFormat string;
    MotorCommand motorMessage;
  } data;
  UART_TRANSMITTER_VARIANT_TYPE type;
};

bool sendToUartQueue(struct UART_TRANSMITTER_VARIANT *var);

#ifdef __cplusplus
}
#endif

#endif /* UART_TRANSMITTER_PUBLIC_H */
