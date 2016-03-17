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

  bool started = false;
  mutable std::mutex mut;

  FRIEND_TEST(RegisterCallbackTest, CallbackCheck);
  FRIEND_TEST(SendToQueueTest, EnqueueTest);
};

#endif /* _RSSIVECTORCONSTRUCTOR_H_ */
