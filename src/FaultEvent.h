#pragma once

#include <string>

#include "HealthState.h"

struct FaultEvent {
    int timestamp_ms;
    HealthState previous_state;
    HealthState current_state;
    std::string reason;
};