
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



#include "WiFlyStringWireFormat.pbo.h"
TEST(WiFlyStringWireFormat_msg, msg_test) {
WiFlyStringWireFormat my_message;
WiFlyStringWireFormat_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);
EXPECT_EQ(false, WiFlyStringWireFormat_has_msg(&my_message));
EXPECT_EQ(0, my_message.msg_size);

      const char *test_str = "The quick brown fox jumps over the lazy dog";
WiFlyStringWireFormat_set_msg(&my_message, test_str, strlen(test_str));
EXPECT_EQ(true, WiFlyStringWireFormat_has_msg(&my_message));

    char buf[150];
WiFlyStringWireFormat_msg(&my_message, buf, sizeof(buf));

    EXPECT_EQ(0, strcmp(buf, test_str));
WiFlyStringWireFormat_clear_msg(&my_message);
EXPECT_EQ(false, WiFlyStringWireFormat_has_msg(&my_message));
EXPECT_EQ(0, my_message.msg_size);

    // Test string truncation
    memset(buf, 3, sizeof(buf));
buf[sizeof(buf) - 1] = 0;
WiFlyStringWireFormat_set_msg(&my_message, buf, strlen(buf));

    char buf2[150];
WiFlyStringWireFormat_msg(&my_message, buf2, sizeof(buf2));

    buf[100] = 0;
EXPECT_EQ(100, strlen(buf2));
EXPECT_EQ(0, strcmp(buf, buf2));
}
