#ifndef _UART_RECEIVER_PUBLIC_H_
#define _UART_RECEIVER_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"
#include "generated/RoverPose.pbo.h"
#include "generated/RSSIData.pbo.h"
#include "SafeQueue.h"

typedef enum {
  DEBUG_INFO,
  TEST_CHAR,
  RSSI_PAIR,
  MOVE_COMPLETE,
  PROFILE_INFO
} UART_RECEIVER_VARIANT_TYPE;

struct __attribute__((packed)) ProfileInfo {
  char name[20];
  uint32_t runtime;
  uint32_t total_runtime;
};

struct __attribute__((packed)) PosePositionPair {
  RoverPose pose;
  RSSIData rssi;
};

struct __attribute__((packed)) UART_RECEIVER_VARIANT {
  union {
    DebugInfo debug_info;
    uint8_t test_char;
    struct PosePositionPair rssi_pair;
    struct ProfileInfo profile_info;
  } data;
  uint8_t type;
};

struct __attribute__((packed)) UART_RECEIVER_VARIANT_WIRE {
  union {
    DebugInfo debug_info;
    uint8_t test_char;
    struct ProfileInfo profile_info;
  } data;
  uint8_t type;
};

typedef int (*uart_receiver_callback_t)(struct UART_RECEIVER_VARIANT *);

int registerUartReceiverCallback(uart_receiver_callback_t callback);

#endif /* _UART_RECEIVER_PUBLIC_H_ */
