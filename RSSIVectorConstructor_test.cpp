#include <sstream>

#include "RSSIVectorConstructor.h"
#include "gtest/gtest.h"

TEST(RegisterCallbackTest, CallbackCheck) {
  RSSIVectorConstructor dut;
  EXPECT_EQ(dut.callbacks.size(), 0);
  bool called = false;
  dut.registerCallback(
      [&](const std::vector<std::tuple<RSSIData, RoverPose> > &x) {
        called = true;
      });
  EXPECT_EQ(dut.callbacks.size(), 1);
  std::vector<std::tuple<RSSIData, RoverPose> > test_val;
  RSSIData rssi;
  RoverPose pose;
  test_val.push_back(std::make_tuple(rssi, pose));
  dut.callbacks[0](test_val);
  EXPECT_EQ(called, true);
}

TEST(SendToQueueTest, EnqueueTest) {
  RSSIVectorConstructor dut;
  EXPECT_EQ(dut.queue.empty(), true);
  std::tuple<RSSIData, RoverPose> test_val;
  RSSIData rssi;
  RoverPose pose;
  EXPECT_EQ(dut.sendToQueue(std::make_tuple(rssi, pose)), true);
  EXPECT_EQ(dut.queue.empty(), false);
  EXPECT_EQ(dut.queue.size(), 1);
}

TEST(SimpleProcessTest, TestProcess) {
  RSSIVectorConstructor dut;

  // Setup data
  RSSIData rssi_data;
  RSSIData_init(&rssi_data);
  RSSIData_set_bssid(&rssi_data, "test", strlen("test"));
  RSSIData_set_rssi(&rssi_data, -123);
  RSSIData_set_pairSeq(&rssi_data, 1337);
  RSSIData_set_frameSize(&rssi_data, 8008);
  RSSIData_set_frameNum(&rssi_data, 1738);
  RSSIData_to_bytes(&rssi_data, (char *)&rssi_data, 0);

  RoverPose pose;
  RoverPose_init(&pose);
  RoverPose_set_xPosition(&pose, 1);
  RoverPose_set_yPosition(&pose, 2);
  RoverPose_set_yaw(&pose, 3);
  RoverPose_to_bytes(&pose, (char *)&pose, 0);

  // Test success path
  auto tuple = std::make_tuple(rssi_data, pose);
  dut.sendToQueue(tuple);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).magic, 0xdeadbeef);
  testing::internal::CaptureStdout();
  dut.process();
  EXPECT_EQ(testing::internal::GetCapturedStdout(),
            "Received RSSI data message:\n"
            "\tBSSID: test\n"
            "\tRSSI: -123\n"
            "\tPair sequence no: 1337\n"
            "\tFrame size: 8008\n"
            "\tFrame num: 1738\n");

  RSSIData_to_bytes(&rssi_data, (char *)&rssi_data, 1);
  RoverPose_to_bytes(&pose, (char *)&pose, 1);
  // Test malformed hashes
  rssi_data.siphash = 0x1234;
  tuple = std::make_tuple(rssi_data, pose);
  dut.sendToQueue(tuple);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).siphash, 0x1234);
  testing::internal::CaptureStderr();
  dut.process();
  EXPECT_EQ(testing::internal::GetCapturedStderr(),
            "RSSIData message integrity check failed!\n");
  RSSIData_to_bytes(&rssi_data, (char *)&rssi_data, 1);

  pose.siphash = 0x4321;
  tuple = std::make_tuple(rssi_data, pose);
  dut.sendToQueue(tuple);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).siphash, 0x4321);
  testing::internal::CaptureStderr();
  dut.process();
  EXPECT_EQ(testing::internal::GetCapturedStderr(),
            "RoverPose message integrity check failed!\n");
  RoverPose_to_bytes(&pose, (char *)&pose, 1);


  // Missing sequence number
  RSSIData_to_bytes(&rssi_data, (char*)&rssi_data, 2);
  tuple = std::make_tuple(rssi_data, pose);
  dut.sendToQueue(tuple);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).seq, 2);
  testing::internal::CaptureStderr();
  dut.process();
  EXPECT_EQ(testing::internal::GetCapturedStderr(),
            "RSSIData sequence number check failed. Expected: 1 Got: 2\n");
  RSSIData_to_bytes(&rssi_data, (char *)&rssi_data, 1);

  RoverPose_to_bytes(&pose, (char*)&pose, 2);
  tuple = std::make_tuple(rssi_data, pose);
  dut.sendToQueue(tuple);
  EXPECT_EQ(std::get<0>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).magic, 0xdeadbeef);
  EXPECT_EQ(std::get<1>(dut.queue.q.front()).seq, 2);
  testing::internal::CaptureStderr();
  dut.process();
  EXPECT_EQ(testing::internal::GetCapturedStderr(),
            "RoverPose sequence number check failed. Expected: 1 Got: 2\n");
  RoverPose_to_bytes(&pose, (char*)&pose, 1);
}

TEST(FullVectorTest, TestFullVector) {
  RSSIVectorConstructor dut;

  // Setup data
  RSSIData rssi_data;
  RSSIData_init(&rssi_data);
  RSSIData_set_bssid(&rssi_data, "test", strlen("test"));
  RSSIData_set_rssi(&rssi_data, -123);
  RSSIData_set_pairSeq(&rssi_data, 0);
  RSSIData_set_frameSize(&rssi_data, 5);
  RSSIData_set_frameNum(&rssi_data, 0);
  RSSIData_to_bytes(&rssi_data, (char *)&rssi_data, 0);

  RoverPose pose;
  RoverPose_init(&pose);
  RoverPose_set_xPosition(&pose, 1);
  RoverPose_set_yPosition(&pose, 2);
  RoverPose_set_yaw(&pose, 3);
  RoverPose_to_bytes(&pose, (char *)&pose, 0);

  auto tuple = std::make_tuple(rssi_data, pose);
}
