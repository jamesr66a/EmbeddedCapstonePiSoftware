
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



#include "IRSensorData.pbo.h"
TEST(IRSensorData_front, front_test){
IRSensorData my_message;
IRSensorData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, IRSensorData_has_front(&my_message));
EXPECT_EQ(0x0000, my_message.front);
IRSensorData_set_front(&my_message, 0x1234);
EXPECT_EQ(true, IRSensorData_has_front(&my_message));
EXPECT_EQ(htons(0x1234), my_message.front);
EXPECT_EQ(0x1234, IRSensorData_front(&my_message));
IRSensorData_clear_front(&my_message);
EXPECT_EQ(false, IRSensorData_has_front(&my_message));
EXPECT_EQ(0x0000, my_message.front);
}
TEST(IRSensorData_left, left_test){
IRSensorData my_message;
IRSensorData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, IRSensorData_has_left(&my_message));
EXPECT_EQ(0x0000, my_message.left);
IRSensorData_set_left(&my_message, 0x1234);
EXPECT_EQ(true, IRSensorData_has_left(&my_message));
EXPECT_EQ(htons(0x1234), my_message.left);
EXPECT_EQ(0x1234, IRSensorData_left(&my_message));
IRSensorData_clear_left(&my_message);
EXPECT_EQ(false, IRSensorData_has_left(&my_message));
EXPECT_EQ(0x0000, my_message.left);
}
TEST(IRSensorData_right, right_test){
IRSensorData my_message;
IRSensorData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, IRSensorData_has_right(&my_message));
EXPECT_EQ(0x0000, my_message.right);
IRSensorData_set_right(&my_message, 0x1234);
EXPECT_EQ(true, IRSensorData_has_right(&my_message));
EXPECT_EQ(htons(0x1234), my_message.right);
EXPECT_EQ(0x1234, IRSensorData_right(&my_message));
IRSensorData_clear_right(&my_message);
EXPECT_EQ(false, IRSensorData_has_right(&my_message));
EXPECT_EQ(0x0000, my_message.right);
}
