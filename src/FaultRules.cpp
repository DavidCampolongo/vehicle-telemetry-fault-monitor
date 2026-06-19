#include "FaultRules.h"

HealthState evaluateHealth(const TelemetryRecord& record) {
    const double BATTERY_CRITICAL_V = 10.0;
    const double BATTERY_WARNING_V = 12.0;

    const double TEMPERATURE_WARNING_C = 75.0;
    const double TEMPERATURE_CRITICAL_C = 90.0;

    const double CURRENT_WARNING_A = 8.0;
    const double CURRENT_CRITICAL_A = 12.0;

    bool link_ok = record.link_status;

    if (!record.sensor_valid) {
        return HealthState::FAULT;
    }

    if (!link_ok) {
        return HealthState::FAULT;
    }

    if (record.battery_voltage < BATTERY_CRITICAL_V) {
        return HealthState::CRITICAL;
    }

    if (record.temperature_c > TEMPERATURE_CRITICAL_C) {
        return HealthState::CRITICAL;
    }

    if (record.current_draw_a > CURRENT_CRITICAL_A) {
        return HealthState::CRITICAL;
    }

    if (record.battery_voltage >= BATTERY_CRITICAL_V &&
        record.battery_voltage <= BATTERY_WARNING_V) {
        return HealthState::WARNING;
    }

    if (record.temperature_c >= TEMPERATURE_WARNING_C &&
        record.temperature_c <= TEMPERATURE_CRITICAL_C) {
        return HealthState::WARNING;
    }

    if (record.current_draw_a > CURRENT_WARNING_A) {
        return HealthState::WARNING;
    }

    return HealthState::NOMINAL;
}