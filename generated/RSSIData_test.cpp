
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



#include "RSSIData.pbo.h"
TEST(RSSIData_bssid, bssid_test) {
RSSIData my_message;
RSSIData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);
EXPECT_EQ(false, RSSIData_has_bssid(&my_message));
EXPECT_EQ(0, my_message.bssid_size);

      const char *test_str = "The quick brown fox jumps over the lazy dog";
RSSIData_set_bssid(&my_message, test_str, strlen(test_str));
EXPECT_EQ(true, RSSIData_has_bssid(&my_message));

    char buf[150];
RSSIData_bssid(&my_message, buf, sizeof(buf));

    EXPECT_EQ(0, strcmp(buf, test_str));
RSSIData_clear_bssid(&my_message);
EXPECT_EQ(false, RSSIData_has_bssid(&my_message));
EXPECT_EQ(0, my_message.bssid_size);

    // Test string truncation
    memset(buf, 3, sizeof(buf));
buf[sizeof(buf) - 1] = 0;
RSSIData_set_bssid(&my_message, buf, strlen(buf));

    char buf2[150];
RSSIData_bssid(&my_message, buf2, sizeof(buf2));

    buf[100] = 0;
EXPECT_EQ(100, strlen(buf2));
EXPECT_EQ(0, strcmp(buf, buf2));
}
TEST(RSSIData_rssi, rssi_test){
RSSIData my_message;
RSSIData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RSSIData_has_rssi(&my_message));
EXPECT_EQ(0x0000, my_message.rssi);
RSSIData_set_rssi(&my_message, 0x1234);
EXPECT_EQ(true, RSSIData_has_rssi(&my_message));
EXPECT_EQ(htons(0x1234), my_message.rssi);
EXPECT_EQ(0x1234, RSSIData_rssi(&my_message));
RSSIData_clear_rssi(&my_message);
EXPECT_EQ(false, RSSIData_has_rssi(&my_message));
EXPECT_EQ(0x0000, my_message.rssi);
}
TEST(RSSIData_pairSeq, pairSeq_test){
RSSIData my_message;
RSSIData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RSSIData_has_pairSeq(&my_message));
EXPECT_EQ(0x00000000, my_message.pairSeq);
RSSIData_set_pairSeq(&my_message, 0x12345678);
EXPECT_EQ(true, RSSIData_has_pairSeq(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.pairSeq);
EXPECT_EQ(0x12345678, RSSIData_pairSeq(&my_message));
RSSIData_clear_pairSeq(&my_message);
EXPECT_EQ(false, RSSIData_has_pairSeq(&my_message));
EXPECT_EQ(0x00000000, my_message.pairSeq);
}
TEST(RSSIData_frameSize, frameSize_test){
RSSIData my_message;
RSSIData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RSSIData_has_frameSize(&my_message));
EXPECT_EQ(0x00000000, my_message.frameSize);
RSSIData_set_frameSize(&my_message, 0x12345678);
EXPECT_EQ(true, RSSIData_has_frameSize(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.frameSize);
EXPECT_EQ(0x12345678, RSSIData_frameSize(&my_message));
RSSIData_clear_frameSize(&my_message);
EXPECT_EQ(false, RSSIData_has_frameSize(&my_message));
EXPECT_EQ(0x00000000, my_message.frameSize);
}
TEST(RSSIData_frameNum, frameNum_test){
RSSIData my_message;
RSSIData_init(&my_message);
EXPECT_EQ(0xDEADBEEF, my_message.magic);EXPECT_EQ(false, RSSIData_has_frameNum(&my_message));
EXPECT_EQ(0x00000000, my_message.frameNum);
RSSIData_set_frameNum(&my_message, 0x12345678);
EXPECT_EQ(true, RSSIData_has_frameNum(&my_message));
EXPECT_EQ(htonl(0x12345678), my_message.frameNum);
EXPECT_EQ(0x12345678, RSSIData_frameNum(&my_message));
RSSIData_clear_frameNum(&my_message);
EXPECT_EQ(false, RSSIData_has_frameNum(&my_message));
EXPECT_EQ(0x00000000, my_message.frameNum);
}
