#include "warning_model.h"

#include "debug.h"

WARNING_MODEL_DATA warning_modelData;

// Public functions

void sendToWarningModelQueue(DebugInfo *info) {
  warning_modelData.input_queue.enqueue(*info);
}

int warning_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(warning_modelData.data_mutex);
  return warning_modelData.total_num;
}

float warning_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(warning_modelData.data_mutex);
  return warning_modelData.rate_per_min;
}

std::vector<DebugInfo> warning_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(warning_modelData.data_mutex);
  return warning_modelData.data_vec;
}

// Internal functions

void WARNING_MODEL_Initialize() {
  warning_modelData.state = WARNING_MODEL_INIT;
}

void WARNING_MODEL_Tasks() {
  switch (warning_modelData.state) {
  case WARNING_MODEL_INIT: {
    warning_modelData.state = WARNING_MODEL_RECEIVE;
    warning_modelData.beginning = std::chrono::system_clock::now();
    warning_modelData.total_num = 0;
    warning_modelData.rate_per_min = 0.0;
  } break;
  case WARNING_MODEL_RECEIVE: {
    auto receivedData = warning_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(warning_modelData.data_mutex);
      if (warning_modelData.data_vec.size() == 50) {
        warning_modelData.data_vec.erase(warning_modelData.data_vec.begin());
      }
      warning_modelData.data_vec.push_back(receivedData);
      warning_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         warning_modelData.beginning).count();
      warning_modelData.rate_per_min =
          60 * ((float)warning_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void warning_model_thread_run() {
  while (true) {
    WARNING_MODEL_Tasks();
  }
}
