#ifndef _POSE_MODEL_H_
#define _POSE_MODEL_H_

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "pose_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  POSE_MODEL_INIT,
  POSE_MODEL_RECEIVE,
} POSE_MODEL_STATES;

typedef struct {
  POSE_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} POSE_MODEL_DATA;

void POSE_MODEL_Initialize();
void POSE_MODEL_Tasks();
void pose_model_thread_run();

#endif /* _POSE_MODEL_H_ */
