#include "FaultRules.h"

namespace {
constexpr double BATTERY_CRITICAL_V = 10.0;
constexpr double BATTERY_WARNING_V = 12.0;

constexpr double TEMPERATURE_WARNING_C = 75.0;
constexpr double TEMPERATURE_CRITICAL_C = 90.0;

constexpr double CURRENT_WARNING_A = 8.0;
constexpr double CURRENT_CRITICAL_A = 12.0;
}

HealthEvaluation evaluateTelemetry(const TelemetryRecord& record) {
    if (!record.sensor_valid) {
        return { HealthState::FAULT, "sensor invalid" };
    }

    if (!record.link_ok) {
        return { HealthState::FAULT, "communication link lost" };
    }

    if (record.battery_voltage < BATTERY_CRITICAL_V) {
        return { HealthState::CRITICAL, "battery voltage below critical threshold" };
    }

    if (record.temperature_c > TEMPERATURE_CRITICAL_C) {
        return { HealthState::CRITICAL, "temperature above critical threshold" };
    }

    if (record.current_draw_a > CURRENT_CRITICAL_A) {
        return { HealthState::CRITICAL, "current draw above critical threshold" };
    }

    if (record.battery_voltage >= BATTERY_CRITICAL_V &&
        record.battery_voltage <= BATTERY_WARNING_V) {
        return { HealthState::WARNING, "battery voltage in warning range" };
    }

    if (record.temperature_c >= TEMPERATURE_WARNING_C &&
        record.temperature_c <= TEMPERATURE_CRITICAL_C) {
        return { HealthState::WARNING, "temperature in warning range" };
    }

    if (record.current_draw_a > CURRENT_WARNING_A) {
        return { HealthState::WARNING, "current draw above warning threshold" };
    }

    return { HealthState::NOMINAL, "all telemetry values nominal" };
}

HealthState evaluateHealth(const TelemetryRecord& record) {
    return evaluateTelemetry(record).state;
}