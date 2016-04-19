#ifndef _POSE_MODEL_PUBLIC_H_
#define _POSE_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

#include <vector>

int pose_aggregate_debug_info_count();
float pose_debug_info_rate_per_minute();

void sendToPoseModelQueue(DebugInfo *info);
std::vector<DebugInfo> pose_aggregate_info_vector();

#endif /* _POSE_MODEL_PUBLIC_H_ */
