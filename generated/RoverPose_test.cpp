
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



#include "RoverPose.pbo.h"
TEST(RoverPose_xPosition, xPosition_test){
RoverPose my_message;
RoverPose_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RoverPose_has_xPosition(&my_message));
EXPECT_EQ(0x00000000, my_message.xPosition);
RoverPose_set_xPosition(&my_message, 0x12345678);
EXPECT_EQ(true, RoverPose_has_xPosition(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.xPosition);
EXPECT_EQ(0x12345678, RoverPose_xPosition(&my_message));
RoverPose_clear_xPosition(&my_message);
EXPECT_EQ(false, RoverPose_has_xPosition(&my_message));
EXPECT_EQ(0x00000000, my_message.xPosition);
}
TEST(RoverPose_yPosition, yPosition_test){
RoverPose my_message;
RoverPose_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RoverPose_has_yPosition(&my_message));
EXPECT_EQ(0x00000000, my_message.yPosition);
RoverPose_set_yPosition(&my_message, 0x12345678);
EXPECT_EQ(true, RoverPose_has_yPosition(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.yPosition);
EXPECT_EQ(0x12345678, RoverPose_yPosition(&my_message));
RoverPose_clear_yPosition(&my_message);
EXPECT_EQ(false, RoverPose_has_yPosition(&my_message));
EXPECT_EQ(0x00000000, my_message.yPosition);
}
TEST(RoverPose_yaw, yaw_test){
RoverPose my_message;
RoverPose_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RoverPose_has_yaw(&my_message));
EXPECT_EQ(0x0000, my_message.yaw);
RoverPose_set_yaw(&my_message, 0x1234);
EXPECT_EQ(true, RoverPose_has_yaw(&my_message));
EXPECT_EQ(htons(0x1234), my_message.yaw);
EXPECT_EQ(0x1234, RoverPose_yaw(&my_message));
RoverPose_clear_yaw(&my_message);
EXPECT_EQ(false, RoverPose_has_yaw(&my_message));
EXPECT_EQ(0x0000, my_message.yaw);
}
