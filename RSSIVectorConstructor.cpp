#include "RSSIVectorConstructor.h"

#include <iostream>

bool RSSIVectorConstructor::sendToQueue(
    const std::tuple<RSSIData, RoverPose> &d) {
  queue.enqueue(d);
}

bool RSSIVectorConstructor::registerCallback(vector_ctor_callback_t cb) {
  std::lock_guard<std::mutex> guard(mut);
  callbacks.push_back(cb);
}

void RSSIVectorConstructor::start() {
  std::lock_guard<std::mutex> guard(mut);
  thread.reset(new std::thread(&RSSIVectorConstructor::worker_fn, this));
}

void RSSIVectorConstructor::process() {
  static uint32_t rssi_seq_expected = 0, pose_seq_expected = 0;
  auto message = queue.dequeue();
  RSSIData rssi_data;
  RoverPose rover_pose;
  uint32_t seq_out;

  if (!RSSIData_from_bytes(&std::get<0>(message), (char *)&rssi_data,
                           &seq_out)) {
    std::cerr << "RSSIData message integrity check failed!" << std::endl;
    return;
  }

  if (seq_out != rssi_seq_expected) {
    std::cerr << "RSSIData sequence number check failed. Expected: "
              << rssi_seq_expected << " Got: " << seq_out << std::endl;
  }

  if (!RoverPose_from_bytes(&std::get<1>(message), (char *)&rover_pose,
                            &seq_out)) {
    std::cerr << "RoverPose message integrity check failed!" << std::endl;
    return;
  }

  if (seq_out != pose_seq_expected) {
    std::cerr << "RoverPose sequence number check failed. Expected: "
              << pose_seq_expected << " Got: " << seq_out << std::endl;
  }

  constexpr size_t bssid_buf_size = 20;
  char bssid_buf[bssid_buf_size];
  RSSIData_bssid(&rssi_data, bssid_buf, bssid_buf_size);

  std::cout << "Received RSSI data message:\n"
            << "\tBSSID: " << std::string(bssid_buf) << "\n"
            << "\tRSSI: " << RSSIData_rssi(&rssi_data) << "\n"
            << "\tPair sequence no: " << RSSIData_pairSeq(&rssi_data) << "\n"
            << "\tFrame size: " << RSSIData_frameSize(&rssi_data) << "\n"
            << "\tFrame num: " << RSSIData_frameNum(&rssi_data) << "\n";
}

void RSSIVectorConstructor::worker_fn() {
  while (true) {
    process();
  }
}
