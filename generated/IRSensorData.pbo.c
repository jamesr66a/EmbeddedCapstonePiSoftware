
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

#include "IRSensorData.pbo.h"

static bool has_field(const IRSensorData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  return ((unsigned)msg->has_field[byte_index] & (1u << (i % CHAR_BIT - 1)));
}

static void set_field_present(IRSensorData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] |= (1u << (i % CHAR_BIT - 1));
}

static void set_field_absent(IRSensorData *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] &= ~(1u << (i % CHAR_BIT - 1));
}

static void check_magic(const IRSensorData *msg) {
  if (msg->magic != htonl(0xDEADBEEFu)) {
    fprintf(stderr, "Invalid magic number for message IRSensorData at %p\n",
            (void *)msg);
  }
}

void IRSensorData_init(IRSensorData *msg) {
  memset(msg, 0, sizeof(*msg));
  // TODO: generate this from a hash at compile time
  msg->magic = htonl(0xDEADBEEFu);
}
/*
 * int32 front
 */
bool IRSensorData_has_front(const IRSensorData *msg) {
  check_magic(msg);
  return has_field(msg, 1);
}
int16_t IRSensorData_front(const IRSensorData *msg) {
  check_magic(msg);
  if (has_field(msg, 1)) {
    return ntohs(msg->front);
  } else {
    fprintf(stderr, "Requested field front from IRSensorData at address %p, "
                    "but message do3s not have the field \n",
            (void *)msg);
    return -1;
  }
}
void IRSensorData_set_front(IRSensorData *msg, int16_t value) {
  check_magic(msg);
  msg->front = htons(value);
  set_field_present(msg, 1);
}
void IRSensorData_clear_front(IRSensorData *msg) {
  check_magic(msg);
  msg->front = 0;
  set_field_absent(msg, 1);
}
/*
 * int32 left
 */
bool IRSensorData_has_left(const IRSensorData *msg) {
  check_magic(msg);
  return has_field(msg, 2);
}
int16_t IRSensorData_left(const IRSensorData *msg) {
  check_magic(msg);
  if (has_field(msg, 2)) {
    return ntohs(msg->left);
  } else {
    fprintf(stderr, "Requested field left from IRSensorData at address %p, but "
                    "message do3s not have the field \n",
            (void *)msg);
    return -1;
  }
}
void IRSensorData_set_left(IRSensorData *msg, int16_t value) {
  check_magic(msg);
  msg->left = htons(value);
  set_field_present(msg, 2);
}
void IRSensorData_clear_left(IRSensorData *msg) {
  check_magic(msg);
  msg->left = 0;
  set_field_absent(msg, 2);
}
/*
 * int32 right
 */
bool IRSensorData_has_right(const IRSensorData *msg) {
  check_magic(msg);
  return has_field(msg, 3);
}
int16_t IRSensorData_right(const IRSensorData *msg) {
  check_magic(msg);
  if (has_field(msg, 3)) {
    return ntohs(msg->right);
  } else {
    fprintf(stderr, "Requested field right from IRSensorData at address %p, "
                    "but message do3s not have the field \n",
            (void *)msg);
    return -1;
  }
}
void IRSensorData_set_right(IRSensorData *msg, int16_t value) {
  check_magic(msg);
  msg->right = htons(value);
  set_field_present(msg, 3);
}
void IRSensorData_clear_right(IRSensorData *msg) {
  check_magic(msg);
  msg->right = 0;
  set_field_absent(msg, 3);
}
void IRSensorData_to_bytes(IRSensorData *msg, char *buf, uint32_t seq) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  msg->seq = seq;
  msg->siphash = siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                           "scary spooky skeletons");
  memmove((void *)buf, (void *)msg, sizeof(*msg));
}

bool IRSensorData_from_bytes(IRSensorData *msg, const char *buf,
                             uint32_t *seq_out) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  memmove((void *)msg, (void *)buf, sizeof(*msg));
  *seq_out = msg->seq;
  return (siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                    "scary spooky skeletons") == msg->siphash);
}
