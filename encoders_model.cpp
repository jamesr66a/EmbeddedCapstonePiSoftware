#include "encoders_model.h"

#include "debug.h"

ENCODERS_MODEL_DATA encoders_modelData;

// Public functions

void sendToEncodersModelQueue(DebugInfo *info) {
  encoders_modelData.input_queue.enqueue(*info);
}

int encoders_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(encoders_modelData.data_mutex);
  auto num = encoders_modelData.total_num;
  return num;
}

float encoders_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(encoders_modelData.data_mutex);
  auto rate = encoders_modelData.rate_per_min;
  return rate;
}

std::vector<DebugInfo> encoders_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(encoders_modelData.data_mutex);
  return encoders_modelData.data_vec;
}

// Internal functions

void ENCODERS_MODEL_Initialize() {
  encoders_modelData.state = ENCODERS_MODEL_INIT;
}

void ENCODERS_MODEL_Tasks() {
  switch (encoders_modelData.state) {
  case ENCODERS_MODEL_INIT: {
    encoders_modelData.state = ENCODERS_MODEL_RECEIVE;
    encoders_modelData.beginning = std::chrono::system_clock::now();
    encoders_modelData.total_num = 0;
    encoders_modelData.rate_per_min = 0.0;
  } break;
  case ENCODERS_MODEL_RECEIVE: {
    auto receivedData = encoders_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(encoders_modelData.data_mutex);
      if (encoders_modelData.data_vec.size() == 50) {
        encoders_modelData.data_vec.erase(encoders_modelData.data_vec.begin());
      }
      encoders_modelData.data_vec.push_back(receivedData);
      encoders_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         encoders_modelData.beginning).count();
      encoders_modelData.rate_per_min =
          60 * ((float)encoders_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void encoders_model_thread_run() {
  while (true) {
    ENCODERS_MODEL_Tasks();
  }
}
