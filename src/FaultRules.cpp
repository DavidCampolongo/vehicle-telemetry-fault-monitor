#include "FaultRules.h"

#include "HealthState.h"

namespace {
constexpr double BATTERY_CRITICAL_V = 10.0;
constexpr double BATTERY_WARNING_V = 12.0;

constexpr double TEMPERATURE_WARNING_C = 75.0;
constexpr double TEMPERATURE_CRITICAL_C = 90.0;

constexpr double CURRENT_WARNING_A = 8.0;
constexpr double CURRENT_CRITICAL_A = 12.0;

void appendReason(std::string& reason, const std::string& newReason) {
    if (!reason.empty()) {
        reason += "; ";
    }

    reason += newReason;
}

void promoteState(HealthState& currentState, HealthState candidateState) {
    if (severityRank(candidateState) > severityRank(currentState)) {
        currentState = candidateState;
    }
}

void addFinding(
    HealthEvaluation& evaluation,
    HealthState state,
    const std::string& reason
) {
    promoteState(evaluation.state, state);
    appendReason(evaluation.reason, reason);
}
}

HealthEvaluation evaluateTelemetry(const TelemetryRecord& record) {
    HealthEvaluation evaluation {
        HealthState::NOMINAL,
        ""
    };

    if (!record.sensor_valid) {
        addFinding(evaluation, HealthState::FAULT, "sensor invalid");
    }

    if (!record.link_ok) {
        addFinding(evaluation, HealthState::FAULT, "communication link lost");
    }

    if (!record.sensor_valid) {
        return evaluation;
    }

    if (record.battery_voltage < BATTERY_CRITICAL_V) {
        addFinding(
            evaluation,
            HealthState::CRITICAL,
            "battery voltage below critical threshold"
        );
    }
    else if (record.battery_voltage <= BATTERY_WARNING_V) {
        addFinding(
            evaluation,
            HealthState::WARNING,
            "battery voltage in warning range"
        );
    }

    if (record.temperature_c > TEMPERATURE_CRITICAL_C) {
        addFinding(
            evaluation,
            HealthState::CRITICAL,
            "temperature above critical threshold"
        );
    }
    else if (record.temperature_c >= TEMPERATURE_WARNING_C) {
        addFinding(
            evaluation,
            HealthState::WARNING,
            "temperature in warning range"
        );
    }

    if (record.current_draw_a > CURRENT_CRITICAL_A) {
        addFinding(
            evaluation,
            HealthState::CRITICAL,
            "current draw above critical threshold"
        );
    }
    else if (record.current_draw_a > CURRENT_WARNING_A) {
        addFinding(
            evaluation,
            HealthState::WARNING,
            "current draw above warning threshold"
        );
    }

    if (evaluation.reason.empty()) {
        evaluation.reason = "all telemetry values nominal";
    }

    return evaluation;
}

HealthState evaluateHealth(const TelemetryRecord& record) {
    return evaluateTelemetry(record).state;
}