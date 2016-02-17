
#ifdef NETWORK_BIG_ENDIAN
#include <arpa/inet.h>
#endif
#include <gtest/gtest.h>
#include <stdint.h>

#ifndef NETWORK_BIG_ENDIAN
static uint32_t htonl(uint32_t hostlong) {
  return hostlong;
}

static uint16_t htons(uint16_t hostshort) {
  return hostshort;
}

static uint32_t ntohl(uint32_t netlong) {
  return netlong;
}

static uint16_t ntohs(uint16_t netshort) {
  return netshort;
}
#endif



#include "DebugInfo.pbo.h"
TEST(DebugInfo_identifier, identifier_test){
DebugInfo my_message;
DebugInfo_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, DebugInfo_has_identifier(&my_message));
EXPECT_EQ(0x00000000, my_message.identifier);
DebugInfo_set_identifier(&my_message, 0x12345678);
EXPECT_EQ(true, DebugInfo_has_identifier(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.identifier);
EXPECT_EQ(0x12345678, DebugInfo_identifier(&my_message));
DebugInfo_clear_identifier(&my_message);
EXPECT_EQ(false, DebugInfo_has_identifier(&my_message));
EXPECT_EQ(0x00000000, my_message.identifier);
}
TEST(DebugInfo_debugID, debugID_test){
DebugInfo my_message;
DebugInfo_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, DebugInfo_has_debugID(&my_message));
EXPECT_EQ(0x00000000, my_message.debugID);
DebugInfo_set_debugID(&my_message, 0x12345678);
EXPECT_EQ(true, DebugInfo_has_debugID(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.debugID);
EXPECT_EQ(0x12345678, DebugInfo_debugID(&my_message));
DebugInfo_clear_debugID(&my_message);
EXPECT_EQ(false, DebugInfo_has_debugID(&my_message));
EXPECT_EQ(0x00000000, my_message.debugID);
}
TEST(DebugInfo_data, data_test){
DebugInfo my_message;
DebugInfo_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, DebugInfo_has_data(&my_message));
EXPECT_EQ(0x00000000, my_message.data);
DebugInfo_set_data(&my_message, 0x12345678);
EXPECT_EQ(true, DebugInfo_has_data(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.data);
EXPECT_EQ(0x12345678, DebugInfo_data(&my_message));
DebugInfo_clear_data(&my_message);
EXPECT_EQ(false, DebugInfo_has_data(&my_message));
EXPECT_EQ(0x00000000, my_message.data);
}
