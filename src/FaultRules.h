#pragma once

#include <string>

#include "TelemetryRecord.h"
#include "HealthState.h"

struct HealthEvaluation {
    HealthState state;
    std::string reason;
};

HealthEvaluation evaluateTelemetry(const TelemetryRecord& record);

HealthState evaluateHealth(const TelemetryRecord& record);