
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

#include "MotorCommand.pbo.h"

static bool has_field(const MotorCommand *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  return ((unsigned)msg->has_field[byte_index] & (1u << (i % CHAR_BIT - 1)));
}

static void set_field_present(MotorCommand *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] |= (1u << (i % CHAR_BIT - 1));
}

static void set_field_absent(MotorCommand *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] &= ~(1u << (i % CHAR_BIT - 1));
}

static void check_magic(const MotorCommand *msg) {
  if (msg->magic != htonl(0xDEADBEEFu)) {
    fprintf(stderr, "Invalid magic number for message MotorCommand at %p\n",
            (void *)msg);
  }
}

void MotorCommand_init(MotorCommand *msg) {
  memset(msg, 0, sizeof(*msg));
  // TODO: generate this from a hash at compile time
  msg->magic = htonl(0xDEADBEEFu);
}
/*
 * int32 direction
 */
bool MotorCommand_has_direction(const MotorCommand *msg) {
  check_magic(msg);
  return has_field(msg, 1);
}
uint8_t MotorCommand_direction(const MotorCommand *msg) {
  check_magic(msg);
  if (has_field(msg, 1)) {
    return (msg->direction);
  } else {
    fprintf(stderr, "Requested field direction from MotorCommand at address "
                    "%p, but message do3s not have the field \n",
            (void *)msg);
    return -1;
  }
}
void MotorCommand_set_direction(MotorCommand *msg, uint8_t value) {
  check_magic(msg);
  msg->direction = (value);
  set_field_present(msg, 1);
}
void MotorCommand_clear_direction(MotorCommand *msg) {
  check_magic(msg);
  msg->direction = 0;
  set_field_absent(msg, 1);
}
/*
 * int32 dutyCycle
 */
bool MotorCommand_has_dutyCycle(const MotorCommand *msg) {
  check_magic(msg);
  return has_field(msg, 2);
}
uint8_t MotorCommand_dutyCycle(const MotorCommand *msg) {
  check_magic(msg);
  if (has_field(msg, 2)) {
    return (msg->dutyCycle);
  } else {
    fprintf(stderr, "Requested field dutyCycle from MotorCommand at address "
                    "%p, but message do3s not have the field \n",
            (void *)msg);
    return -1;
  }
}
void MotorCommand_set_dutyCycle(MotorCommand *msg, uint8_t value) {
  check_magic(msg);
  msg->dutyCycle = (value);
  set_field_present(msg, 2);
}
void MotorCommand_clear_dutyCycle(MotorCommand *msg) {
  check_magic(msg);
  msg->dutyCycle = 0;
  set_field_absent(msg, 2);
}
/*
 * int32 mode
 */
bool MotorCommand_has_mode(const MotorCommand *msg) {
  check_magic(msg);
  return has_field(msg, 3);
}
uint8_t MotorCommand_mode(const MotorCommand *msg) {
  check_magic(msg);
  if (has_field(msg, 3)) {
    return (msg->mode);
  } else {
    fprintf(stderr, "Requested field mode from MotorCommand at address %p, but "
                    "message do3s not have the field \n",
            (void *)msg);
    return -1;
  }
}
void MotorCommand_set_mode(MotorCommand *msg, uint8_t value) {
  check_magic(msg);
  msg->mode = (value);
  set_field_present(msg, 3);
}
void MotorCommand_clear_mode(MotorCommand *msg) {
  check_magic(msg);
  msg->mode = 0;
  set_field_absent(msg, 3);
}
void MotorCommand_to_bytes(MotorCommand *msg, char *buf, uint32_t seq) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  msg->seq = seq;
  msg->siphash = siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                           "scary spooky skeletons");
  memmove((void *)buf, (void *)msg, sizeof(*msg));
}

bool MotorCommand_from_bytes(MotorCommand *msg, const char *buf,
                             uint32_t *seq_out) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  memmove((void *)msg, (void *)buf, sizeof(*msg));
  *seq_out = msg->seq;
  return (siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                    "scary spooky skeletons") == msg->siphash);
}
