#include "rssi_model.h"

#include "debug.h"

RSSI_MODEL_DATA rssi_modelData;

// Public functions

void sendToRSSIModelQueue(DebugInfo *info) {
  rssi_modelData.input_queue.enqueue(*info);
}

int rssi_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(rssi_modelData.data_mutex);
  return rssi_modelData.total_num;
}

int rssi_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(rssi_modelData.data_mutex);
  return rssi_modelData.rate_per_min;
}

std::vector<DebugInfo> rssi_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(rssi_modelData.data_mutex);
  return rssi_modelData.data_vec;
}

// Internal functions

void RSSI_MODEL_Initialize() { rssi_modelData.state = RSSI_MODEL_INIT; }

void RSSI_MODEL_Tasks() {
  switch (rssi_modelData.state) {
  case RSSI_MODEL_INIT: {
    rssi_modelData.state = RSSI_MODEL_RECEIVE;
    rssi_modelData.beginning = std::chrono::system_clock::now();
    rssi_modelData.total_num = 0;
    rssi_modelData.rate_per_min = 0.0;
  } break;
  case RSSI_MODEL_RECEIVE: {
    auto receivedData = rssi_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(rssi_modelData.data_mutex);
      if (rssi_modelData.data_vec.size() == 50) {
        rssi_modelData.data_vec.erase(rssi_modelData.data_vec.begin());
      }
      rssi_modelData.data_vec.push_back(receivedData);
      rssi_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         rssi_modelData.beginning).count();
      rssi_modelData.rate_per_min =
          60 * ((float)rssi_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void rssi_model_thread_run() {
  while (true) {
    RSSI_MODEL_Tasks();
  }
}
