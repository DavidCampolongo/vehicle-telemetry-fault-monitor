#pragma once

#include <string>

enum class HealthState {
    INIT,
    NOMINAL,
    WARNING,
    CRITICAL,
    FAULT,
};

std::string toString(HealthState state);

int severityRank(HealthState state);