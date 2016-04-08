#ifndef _WARNING_MODEL_PUBLIC_H_
#define _WARNING_MODEL_PUBLIC_H_

#include "generated/DebugInfo.pbo.h"

#include <vector>

int warning_aggregate_debug_info_count();
float warning_debug_info_rate_per_minute();

void sendToWarningModelQueue(DebugInfo *info);
std::vector<DebugInfo> warning_aggregate_info_vector();

#endif /* _WARNING_MODEL_PUBLIC_H_ */
