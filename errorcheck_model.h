#ifndef _ERRORCHECK_MODEL_H_
#define _ERRORCHECK_MODEL_H_

#include "generated/DebugInfo.pbo.h"
#include "SafeQueue.h"
#include "errorcheck_model_public.h"

#include <chrono>
#include <mutex>
#include <vector>

typedef enum {
  ERRORCHECK_MODEL_INIT,
  ERRORCHECK_MODEL_RECEIVE,
} ERRORCHECK_MODEL_STATES;

typedef struct {
  ERRORCHECK_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} ERRORCHECK_MODEL_DATA;

void ERRORCHECK_MODEL_Initialize();
void ERRORCHECK_MODEL_Tasks();
void errorcheck_model_thread_run();

#endif /* _ERRORCHECK_MODEL_H_ */
