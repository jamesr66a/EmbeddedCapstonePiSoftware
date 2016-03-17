#ifndef _RSSIVECTORCONSTRUCTOR_H_
#define _RSSIVECTORCONSTRUCTOR_H_

#include "generated/RSSIData.pbo.h"
#include "generated/RoverPose.pbo.h"
#include "gtest/gtest.h"
#include "SafeQueue.h"

#include <functional>
#include <memory>
#include <mutex>
#include <thread>
#include <unordered_map>
#include <vector>

typedef std::function<void(
    const std::vector<std::tuple<RSSIData, RoverPose> > &)>
    vector_ctor_callback_t;

class RSSIVectorConstructor {
public:
  bool sendToQueue(const std::tuple<RSSIData, RoverPose> &d);
  bool registerCallback(vector_ctor_callback_t cb);
  void start();

private:
  void worker_fn();
  void process();

  std::unique_ptr<std::thread> thread;
  SafeQueue<std::tuple<RSSIData, RoverPose> > queue;

  std::vector<vector_ctor_callback_t> callbacks;

  mutable std::mutex mut;
  bool started = false;
  std::vector<std::tuple<RSSIData, RoverPose> > current_vector;
  int32_t current_framenum = 0;

  uint32_t rssi_seq_expected = 0, pose_seq_expected = 0;

  FRIEND_TEST(RegisterCallbackTest, CallbackCheck);
  FRIEND_TEST(SendToQueueTest, EnqueueTest);
  FRIEND_TEST(SimpleProcessTest, TestProcess);
  FRIEND_TEST(FullVectorTest, TestFullVector);
};

#endif /* _RSSIVECTORCONSTRUCTOR_H_ */
