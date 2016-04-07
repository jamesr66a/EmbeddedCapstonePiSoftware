#ifndef _PID_MODEL_PUBLIC_H_
#define _PID_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

#include <vector>

int pid_aggregate_debug_info_count();
int pid_debug_info_rate_per_minute();

void sendToPIDModelQueue(DebugInfo *info);
std::vector<DebugInfo> pid_aggregate_info_vector();
 
#endif /* _PID_MODEL_PUBLIC_H_ */
