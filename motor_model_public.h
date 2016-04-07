#ifndef _MOTOR_MODEL_PUBLIC_H_
#define _MOTOR_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

#include <vector>

int motor_aggregate_debug_info_count();
int motor_debug_info_rate_per_minute();

void sendToMOTORModelQueue(DebugInfo *info);
std::vector<DebugInfo> motor_aggregate_info_vector();
 
#endif /* _MOTOR_MODEL_PUBLIC_H_ */
