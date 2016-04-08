#ifndef _WARNING_MODEL_H_
#define _WARNING_MODEL_H_

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "warning_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  WARNING_MODEL_INIT,
  WARNING_MODEL_RECEIVE,
} WARNING_MODEL_STATES;

typedef struct {
  WARNING_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} WARNING_MODEL_DATA;

void WARNING_MODEL_Initialize();
void WARNING_MODEL_Tasks();
void warning_model_thread_run();

#endif /* _WARNING_MODEL_H_ */
