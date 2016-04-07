#ifndef _SENSORS_MODEL_PUBLIC_H_
#define _SENSORS_MODEL_PUBLIC_H_ 

#include "generated/DebugInfo.pbo.h"

#include <vector>

int sensors_aggregate_debug_info_count();
float sensors_debug_info_rate_per_minute();

void sendToSensorsModelQueue(DebugInfo *info);
std::vector<DebugInfo> sensors_aggregate_info_vector();

#endif /* _SENSORS_MODEL_PUBLIC_H_ */
