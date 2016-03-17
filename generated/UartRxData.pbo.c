
/*****************************************************************************
 *                   AUTOMATICALLY GENERATED FILE. DO NOT EDIT.              *
 ****************************************************************************/

#ifdef NETWORK_BIG_ENDIAN
#include <arpa/inet.h>
#endif
#include <limits.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint64_t siphash24(const void *src, unsigned long src_sz, const char key[16]);

#ifndef NETWORK_BIG_ENDIAN
static uint32_t htonl(uint32_t hostlong) { return hostlong; }

static uint16_t htons(uint16_t hostshort) { return hostshort; }

static uint32_t ntohl(uint32_t netlong) { return netlong; }

static uint16_t ntohs(uint16_t netshort) { return netshort; }
#endif

#include "UartRxData.pbo.h"

static bool has_field(const UartRxData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  return ((unsigned)msg->has_field[byte_index] & (1u << (i % CHAR_BIT - 1)));
}

static void set_field_present(UartRxData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] |= (1u << (i % CHAR_BIT - 1));
}

static void set_field_absent(UartRxData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] &= ~(1u << (i % CHAR_BIT - 1));
}

static void check_magic(const UartRxData *msg) {
  if (msg->magic != htonl(0xDEADBEEFu)) {
    fprintf(stderr, "Invalid magic number for message UartRxData at %p\n",
            (void *)msg);
  }
}

void UartRxData_init(UartRxData *msg) {
  memset(msg, 0, sizeof(*msg));
  // TODO: generate this from a hash at compile time
  msg->magic = htonl(0xDEADBEEFu);
}
/*
 * int32 charData
 */
bool UartRxData_has_charData(const UartRxData *msg) {
  check_magic(msg);
  return has_field(msg, 1);
}
uint8_t UartRxData_charData(const UartRxData *msg) {
  check_magic(msg);
  if (has_field(msg, 1)) {
    return (msg->charData);
  } else {
    fprintf(stderr, "Requested field charData from UartRxData at address %p, "
                    "but message dows not have the field \n",
            (void *)msg);
  }
}
void UartRxData_set_charData(UartRxData *msg, uint8_t value) {
  check_magic(msg);
  msg->charData = (value);
  set_field_present(msg, 1);
}
void UartRxData_clear_charData(UartRxData *msg) {
  check_magic(msg);
  msg->charData = 0;
  set_field_absent(msg, 1);
}
void UartRxData_to_bytes(UartRxData *msg, char *buf, uint32_t seq) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  msg->seq = seq;
  msg->siphash = siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                           "scary spooky skeletons");
  memmove((void *)buf, (void *)msg, sizeof(*msg));
}

bool UartRxData_from_bytes(UartRxData *msg, const char *buf,
                           uint32_t *seq_out) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  memmove((void *)msg, (void *)buf, sizeof(*msg));
  *seq_out = msg->seq;
  return (siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                    "scary spooky skeletons") == msg->siphash);
}
