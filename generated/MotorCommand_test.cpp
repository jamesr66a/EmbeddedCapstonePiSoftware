
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



#include "MotorCommand.pbo.h"
TEST(MotorCommand_direction, direction_test){
MotorCommand my_message;
MotorCommand_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, MotorCommand_has_direction(&my_message));
EXPECT_EQ(0x00, my_message.direction);
MotorCommand_set_direction(&my_message, 0x12);
EXPECT_EQ(true, MotorCommand_has_direction(&my_message));
EXPECT_EQ((0x12), my_message.direction);
EXPECT_EQ(0x12, MotorCommand_direction(&my_message));
MotorCommand_clear_direction(&my_message);
EXPECT_EQ(false, MotorCommand_has_direction(&my_message));
EXPECT_EQ(0x00, my_message.direction);
}
TEST(MotorCommand_dutyCycle, dutyCycle_test){
MotorCommand my_message;
MotorCommand_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, MotorCommand_has_dutyCycle(&my_message));
EXPECT_EQ(0x00, my_message.dutyCycle);
MotorCommand_set_dutyCycle(&my_message, 0x12);
EXPECT_EQ(true, MotorCommand_has_dutyCycle(&my_message));
EXPECT_EQ((0x12), my_message.dutyCycle);
EXPECT_EQ(0x12, MotorCommand_dutyCycle(&my_message));
MotorCommand_clear_dutyCycle(&my_message);
EXPECT_EQ(false, MotorCommand_has_dutyCycle(&my_message));
EXPECT_EQ(0x00, my_message.dutyCycle);
}
TEST(MotorCommand_mode, mode_test){
MotorCommand my_message;
MotorCommand_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, MotorCommand_has_mode(&my_message));
EXPECT_EQ(0x00, my_message.mode);
MotorCommand_set_mode(&my_message, 0x12);
EXPECT_EQ(true, MotorCommand_has_mode(&my_message));
EXPECT_EQ((0x12), my_message.mode);
EXPECT_EQ(0x12, MotorCommand_mode(&my_message));
MotorCommand_clear_mode(&my_message);
EXPECT_EQ(false, MotorCommand_has_mode(&my_message));
EXPECT_EQ(0x00, my_message.mode);
}
