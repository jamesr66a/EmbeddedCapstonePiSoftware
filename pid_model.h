#ifndef _PID_MODEL_H_
#define _PID_MODEL_H_ 

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "pid_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  PID_MODEL_INIT,
  PID_MODEL_RECEIVE,
} PID_MODEL_STATES;

typedef struct {
  PID_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} PID_MODEL_DATA;

void PID_MODEL_Initialize();
void PID_MODEL_Tasks();
void pid_model_thread_run();

#endif /* _PID_MODEL_H_ */
