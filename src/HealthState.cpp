#include "HealthState.h"

std::string toString(HealthState state) {
    switch (state) {
    case HealthState::INIT:
        return "INIT";
    case HealthState::NOMINAL:
        return "NOMINAL";
    case HealthState::WARNING:
        return "WARNING";
    case HealthState::CRITICAL:
        return "CRITICAL";
    case HealthState::FAULT:
        return "FAULT";
    }

    return "UNKNOWN";
}

int severityRank(HealthState state) {
    switch (state) {
    case HealthState::INIT:
        return 0;
    case HealthState::NOMINAL:
        return 1;
    case HealthState::WARNING:
        return 2;
    case HealthState::CRITICAL:
        return 3;
    case HealthState::FAULT:
        return 4;
    }

    return -1;
}