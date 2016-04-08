#ifndef _WEBSERVER_MODEL_H_
#define _WEBSERVER_MODEL_H_

#include "generated/DebugInfo.pbo.h"
#include "generated/WiFlyStringWireFormat.pbo.h"
#include "SafeQueue.h"
#include "webserver_model_public.h"
#include <chrono>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

typedef enum {
  WEBSERVER_MODEL_INIT,
  WEBSERVER_MODEL_RECEIVE,
} WEBSERVER_MODEL_STATES;

typedef struct {
  WEBSERVER_MODEL_STATES state;

  SafeQueue<DebugInfo> input_queue;

  std::mutex data_mutex;
  std::vector<DebugInfo> data_vec;
  int total_num;
  float rate_per_min;

  std::chrono::system_clock::time_point beginning;
} WEBSERVER_MODEL_DATA;

void WEBSERVER_MODEL_Initialize();
void WEBSERVER_MODEL_Tasks();
void webserver_model_thread_run();

#endif /* _WEBSERVER_MODEL_H_ */
