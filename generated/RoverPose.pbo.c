
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

#include "RoverPose.pbo.h"

static bool has_field(const RoverPose *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  return ((unsigned)msg->has_field[byte_index] & (1u << (i % CHAR_BIT - 1)));
}

static void set_field_present(RoverPose *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] |= (1u << (i % CHAR_BIT - 1));
}

static void set_field_absent(RoverPose *msg, size_t i) {
  size_t byte_index = i / CHAR_BIT;
  msg->has_field[byte_index] &= ~(1u << (i % CHAR_BIT - 1));
}

static void check_magic(const RoverPose *msg) {
  if (msg->magic != htonl(0xDEADBEEFu)) {
    fprintf(stderr, "Invalid magic number for message RoverPose at %p\n",
            (void *)msg);
  }
}

void RoverPose_init(RoverPose *msg) {
  memset(msg, 0, sizeof(*msg));
  // TODO: generate this from a hash at compile time
  msg->magic = htonl(0xDEADBEEFu);
}
/*
 * int32 xPosition
 */
bool RoverPose_has_xPosition(const RoverPose *msg) {
  check_magic(msg);
  return has_field(msg, 1);
}
int32_t RoverPose_xPosition(const RoverPose *msg) {
  check_magic(msg);
  if (has_field(msg, 1)) {
    return ntohl(msg->xPosition);
  } else {
    fprintf(stderr, "Requested field xPosition from RoverPose at address %p, "
                    "but message dows not have the field \n",
            (void *)msg);
  }
}
void RoverPose_set_xPosition(RoverPose *msg, int32_t value) {
  check_magic(msg);
  msg->xPosition = htonl(value);
  set_field_present(msg, 1);
}
void RoverPose_clear_xPosition(RoverPose *msg) {
  check_magic(msg);
  msg->xPosition = 0;
  set_field_absent(msg, 1);
}
/*
 * int32 yPosition
 */
bool RoverPose_has_yPosition(const RoverPose *msg) {
  check_magic(msg);
  return has_field(msg, 2);
}
int32_t RoverPose_yPosition(const RoverPose *msg) {
  check_magic(msg);
  if (has_field(msg, 2)) {
    return ntohl(msg->yPosition);
  } else {
    fprintf(stderr, "Requested field yPosition from RoverPose at address %p, "
                    "but message dows not have the field \n",
            (void *)msg);
  }
}
void RoverPose_set_yPosition(RoverPose *msg, int32_t value) {
  check_magic(msg);
  msg->yPosition = htonl(value);
  set_field_present(msg, 2);
}
void RoverPose_clear_yPosition(RoverPose *msg) {
  check_magic(msg);
  msg->yPosition = 0;
  set_field_absent(msg, 2);
}
/*
 * int32 yaw
 */
bool RoverPose_has_yaw(const RoverPose *msg) {
  check_magic(msg);
  return has_field(msg, 3);
}
int16_t RoverPose_yaw(const RoverPose *msg) {
  check_magic(msg);
  if (has_field(msg, 3)) {
    return ntohs(msg->yaw);
  } else {
    fprintf(stderr, "Requested field yaw from RoverPose at address %p, but "
                    "message dows not have the field \n",
            (void *)msg);
  }
}
void RoverPose_set_yaw(RoverPose *msg, int16_t value) {
  check_magic(msg);
  msg->yaw = htons(value);
  set_field_present(msg, 3);
}
void RoverPose_clear_yaw(RoverPose *msg) {
  check_magic(msg);
  msg->yaw = 0;
  set_field_absent(msg, 3);
}
void RoverPose_to_bytes(RoverPose *msg, char *buf, uint32_t seq) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  msg->seq = seq;
  msg->siphash = siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                           "scary spooky skeletons");
  memmove((void *)buf, (void *)msg, sizeof(*msg));
}

bool RoverPose_from_bytes(RoverPose *msg, const char *buf, uint32_t *seq_out) {
  size_t offset = sizeof(msg->magic) + sizeof(msg->siphash);
  memmove((void *)msg, (void *)buf, sizeof(*msg));
  *seq_out = msg->seq;
  return (siphash24(((char *)msg) + offset, sizeof(*msg) - offset,
                    "scary spooky skeletons") == msg->siphash);
}
