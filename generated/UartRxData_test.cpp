
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



#include "UartRxData.pbo.h"
TEST(UartRxData_charData, charData_test){
UartRxData my_message;
UartRxData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, UartRxData_has_charData(&my_message));
EXPECT_EQ(0x00, my_message.charData);
UartRxData_set_charData(&my_message, 0x12);
EXPECT_EQ(true, UartRxData_has_charData(&my_message));
EXPECT_EQ((0x12), my_message.charData);
EXPECT_EQ(0x12, UartRxData_charData(&my_message));
UartRxData_clear_charData(&my_message);
EXPECT_EQ(false, UartRxData_has_charData(&my_message));
EXPECT_EQ(0x00, my_message.charData);
}
