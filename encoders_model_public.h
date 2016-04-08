#ifndef _ENCODERS_MODEL_PUBLIC_H_
#define _ENCODERS_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

#include <vector>

int encoders_aggregate_debug_info_count();
float encoders_debug_info_rate_per_minute();

void sendToEncodersModelQueue(DebugInfo *info);
std::vector<DebugInfo> encoders_aggregate_info_vector();

#endif /* _ENCODERS_MODEL_PUBLIC_H_ */
