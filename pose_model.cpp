#include "pose_model.h"

#include "debug.h"

POSE_MODEL_DATA pose_modelData;

// Public functions

void sendToPoseModelQueue(DebugInfo *info) {
  pose_modelData.input_queue.enqueue(*info);
}

int pose_aggregate_debug_info_count() {
  std::lock_guard<std::mutex> guard(pose_modelData.data_mutex);
  auto num = pose_modelData.total_num;
  return num;
}

float pose_debug_info_rate_per_minute() {
  std::lock_guard<std::mutex> guard(pose_modelData.data_mutex);
  auto rate = pose_modelData.rate_per_min;
  return rate;
}

std::vector<DebugInfo> pose_aggregate_info_vector() {
  std::lock_guard<std::mutex> guard(pose_modelData.data_mutex);
  return pose_modelData.data_vec;
}

// Internal functions

void POSE_MODEL_Initialize() {
  pose_modelData.state = POSE_MODEL_INIT;
}

void POSE_MODEL_Tasks() {
  switch (pose_modelData.state) {
  case POSE_MODEL_INIT: {
    pose_modelData.state = POSE_MODEL_RECEIVE;
    pose_modelData.beginning = std::chrono::system_clock::now();
    pose_modelData.total_num = 0;
    pose_modelData.rate_per_min = 0.0;
  } break;
  case POSE_MODEL_RECEIVE: {
    auto receivedData = pose_modelData.input_queue.dequeue();

    {
      std::lock_guard<std::mutex> guard(pose_modelData.data_mutex);
      if (pose_modelData.data_vec.size() == 50) {
        pose_modelData.data_vec.erase(pose_modelData.data_vec.begin());
      }
      pose_modelData.data_vec.push_back(receivedData);
      pose_modelData.total_num++;
      auto seconds = std::chrono::duration_cast<std::chrono::seconds>(
                         std::chrono::system_clock::now() -
                         pose_modelData.beginning).count();
      pose_modelData.rate_per_min =
          60 * ((float)pose_modelData.total_num / seconds);
    }
  } break;
  default: { errorCheck(); } break;
  }
}

void pose_model_thread_run() {
  while (true) {
    POSE_MODEL_Tasks();
  }
}
