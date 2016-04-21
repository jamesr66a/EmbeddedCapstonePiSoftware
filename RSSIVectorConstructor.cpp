#include "RSSIVectorConstructor.h"

#include <iostream>

bool RSSIVectorConstructor::sendToQueue(
    const std::tuple<RSSIData, RoverPose> &d) {
  queue.enqueue(d);
  return true;
}

bool RSSIVectorConstructor::registerCallback(vector_ctor_callback_t cb) {
  std::lock_guard<std::mutex> guard(mut);
  callbacks.push_back(cb);
  return true;
}

void RSSIVectorConstructor::start() {
  std::lock_guard<std::mutex> guard(mut);
  thread.reset(new std::thread(&RSSIVectorConstructor::worker_fn, this));
}

void RSSIVectorConstructor::process() {
  auto message = queue.dequeue();
  RSSIData rssi_data;
  RoverPose rover_pose;
  uint32_t seq_out;

  if (!RSSIData_from_bytes(&rssi_data, (char *)&std::get<0>(message),
                           &seq_out)) {
    std::cerr << "RSSIData message integrity check failed!" << std::endl;
    return;
  }

  if (seq_out != rssi_seq_expected) {
    std::cerr << "RSSIData sequence number check failed. Expected: "
              << rssi_seq_expected << " Got: " << seq_out << std::endl;
    rssi_seq_expected = seq_out + 1;
  } else {
    rssi_seq_expected++;
  }

  if (!RoverPose_from_bytes(&rover_pose, (char *)&std::get<1>(message),
                            &seq_out)) {
    std::cerr << "RoverPose message integrity check failed!" << std::endl;
    return;
  }

  if (seq_out != pose_seq_expected) {
    std::cerr << "RoverPose sequence number check failed. Expected: "
              << pose_seq_expected << " Got: " << seq_out << std::endl;
    pose_seq_expected = seq_out + 1;
  } else {
    pose_seq_expected++;
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

  std::cout << "Received rover pose message:\n"
            << "\tx-position: " << RoverPose_xPosition(&rover_pose) << "\n"
            << "\ty-position: " << RoverPose_yPosition(&rover_pose) << "\n"
            << "\tyaw: " << RoverPose_yaw(&rover_pose) << "\n";

  if (RSSIData_frameNum(&rssi_data) != current_framenum) {
    std::cerr << "Inconsistent feature vector frame number. Discarding stored "
                 "vector\n";
    current_framenum = RSSIData_frameNum(&rssi_data);
    current_vector.clear();
  }

  current_vector.push_back(message);
  if (current_vector.size() == (size_t)RSSIData_frameSize(&rssi_data)) {
    std::cout << "Completed a feature vector. Emitting to callbacks\n";
    std::lock_guard<std::mutex> lk(mut);
    for (const auto fn : callbacks) {
      fn(current_vector);
    }
    current_vector.clear();
    current_framenum++;
  }
}

void RSSIVectorConstructor::worker_fn() {
  while (true) {
    process();
  }
}
