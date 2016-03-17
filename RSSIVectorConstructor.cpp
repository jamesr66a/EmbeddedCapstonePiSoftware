#include "RSSIVectorConstructor.h"

bool RSSIVectorConstructor::sendToQueue(
    const std::tuple<RSSIData, RoverPose> &d) {
  queue.enqueue(d);
}

bool RSSIVectorConstructor::registerCallback(vector_ctor_callback_t cb) {
  std::lock_guard guard(mut);
  callbacks.push_back(cb);
}

void RSSIVectorConstructor::start() {
  std::lock_guard guard(mut);
  thread.reset(new std::thread(worker_fn, this));
}

void RSSIVectorConstructor::process() {
  uint32_t seq_expected = 0;
  while (true) {
    auto message = queue.dequeue();
    uint32_t seq_out;
    
    if (!RSSIData_from_bytes(&message, (char*)&message, &seq_out) {
      std::cerr
          << "std::tuple<RSSIData, RoverPose> message integrity check failed!"
          << std::endl;
      continue;
    }

    if (seq_out != seq_expected) {
      std::cerr << "Sequence number check failed. Expected: " << seq_expected
                << " Got: " << seq_out << std::endl;
    }

    constexpr size_t bssid_buf_size = 20;
    char bssid_buf[bssid_buf_size];
    RSSIData_bssid(&message, bssid_buf, bssid_buf_size);

    std::cout << "Received RSSI data message:\n"
              << "\tBSSID: " << std::string(bssid_buf) << "\n"
              << "\tRSSI: " << RSSIData_rssi(&message) << "\n";
  }
}
