#include <cassert>
#include <exception>
#include <string>
#include <vector>

#include "FaultMonitor.h"
#include "FaultRules.h"
#include "TelemetryRecord.h"

void testParseValidTelemetryLine() {
    TelemetryRecord record =
        ParseTelemetryLine("1000,12.6,35.0,4.2,true,true");

    assert(record.timestamp_ms == 1000);
    assert(record.battery_voltage == 12.6);
    assert(record.temperature_c == 35.0);
    assert(record.current_draw_a == 4.2);
    assert(record.link_ok == true);
    assert(record.sensor_valid == true);
}

void testRejectsInvalidNumericField() {
    bool threw = false;

    try {
        ParseTelemetryLine("1000,12x,35.0,4.2,true,true");
    }
    catch (const std::exception&) {
        threw = true;
    }

    assert(threw);
}

void testRejectsExtraCsvField() {
    bool threw = false;

    try {
        ParseTelemetryLine("1000,12.6,35.0,4.2,true,true,extra");
    }
    catch (const std::exception&) {
        threw = true;
    }

    assert(threw);
}

void testFaultRules() {
    TelemetryRecord nominal { 0, 12.6, 35.0, 4.2, true, true };
    TelemetryRecord warning { 1000, 10.5, 78.0, 5.0, true, true };
    TelemetryRecord critical { 2000, 12.1, 92.0, 7.5, true, true };
    TelemetryRecord fault { 3000, 12.0, 45.0, 4.0, false, true };

    assert(evaluateTelemetry(nominal).state == HealthState::NOMINAL);
    assert(evaluateTelemetry(warning).state == HealthState::WARNING);
    assert(evaluateTelemetry(critical).state == HealthState::CRITICAL);
    assert(evaluateTelemetry(fault).state == HealthState::FAULT);
}

void testReportsMultipleWarningReasons() {
    TelemetryRecord record {
        1000,
        10.5,   // battery warning
        78.0,   // temperature warning
        5.0,
        true,
        true
    };

    HealthEvaluation evaluation = evaluateTelemetry(record);

    assert(evaluation.state == HealthState::WARNING);
    assert(evaluation.reason.find("battery voltage") != std::string::npos);
    assert(evaluation.reason.find("temperature") != std::string::npos);
}

void testCriticalOverridesWarningButKeepsBothReasons() {
    TelemetryRecord record {
        1000,
        10.5,   // battery warning
        92.0,   // temperature critical
        5.0,
        true,
        true
    };

    HealthEvaluation evaluation = evaluateTelemetry(record);

    assert(evaluation.state == HealthState::CRITICAL);
    assert(evaluation.reason.find("battery voltage") != std::string::npos);
    assert(evaluation.reason.find("temperature") != std::string::npos);
}

void testFaultMonitorSummary() {
    std::vector<TelemetryRecord> records {
        { 0, 12.6, 35.0, 4.2, true, true },
        { 1000, 10.5, 78.0, 5.0, true, true },
        { 2000, 12.1, 92.0, 7.5, true, true },
        { 3000, 12.0, 45.0, 4.0, false, true }
    };

    FaultMonitor monitor;
    monitor.processRecords(records);

    const MonitorSummary& summary = monitor.getSummary();

    assert(summary.records_processed == 4);
    assert(summary.nominal_count == 1);
    assert(summary.warning_count == 1);
    assert(summary.critical_count == 1);
    assert(summary.fault_count == 1);
    assert(summary.worst_state == HealthState::FAULT);
    assert(summary.events.size() == 3);
}

int main() {
    testParseValidTelemetryLine();
    testRejectsInvalidNumericField();
    testRejectsExtraCsvField();
    testFaultRules();
    testReportsMultipleWarningReasons();
    testCriticalOverridesWarningButKeepsBothReasons();
    testFaultMonitorSummary();

    return 0;
}