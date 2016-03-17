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
