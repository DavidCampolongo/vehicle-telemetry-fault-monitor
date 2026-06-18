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