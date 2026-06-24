#pragma once

#include <vector>

#include "FaultEvent.h"
#include "HealthState.h"
#include "TelemetryRecord.h"

struct MonitorSummary {
    int records_processed = 0;

    int nominal_count = 0;
    int warning_count = 0;
    int critical_count = 0;
    int fault_count = 0;

    HealthState worst_state = HealthState::INIT;

    std::vector<FaultEvent> events;
};

class FaultMonitor {
public:
    void processRecord(const TelemetryRecord& record);
    void processRecords(const std::vector<TelemetryRecord>& records);

    const MonitorSummary& getSummary() const;

private:
    HealthState previous_state_ = HealthState::INIT;
    MonitorSummary summary_;

    void incrementStateCount(HealthState state);
};