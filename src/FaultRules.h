#pragma once

#include "TelemetryRecord.h"
#include "HealthState.h"

HealthState evaluateHealth(const TelemetryRecord& record);