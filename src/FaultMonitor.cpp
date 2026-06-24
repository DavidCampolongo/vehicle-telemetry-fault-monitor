#include "FaultMonitor.h"

#include "FaultRules.h"

void FaultMonitor::processRecord(const TelemetryRecord& record) {
    HealthEvaluation evaluation = evaluateTelemetry(record);

    ++summary_.records_processed;
    incrementStateCount(evaluation.state);

    if (severityRank(evaluation.state) > severityRank(summary_.worst_state)) {
        summary_.worst_state = evaluation.state;
    }

    if (evaluation.state != HealthState::NOMINAL) {
        FaultEvent event {
            record.timestamp_ms,
            previous_state_,
            evaluation.state,
            evaluation.reason
        };

        summary_.events.push_back(event);
    }

    previous_state_ = evaluation.state;
}

void FaultMonitor::processRecords(const std::vector<TelemetryRecord>& records) {
    for (const TelemetryRecord& record : records) {
        processRecord(record);
    }
}

const MonitorSummary& FaultMonitor::getSummary() const {
    return summary_;
}

void FaultMonitor::incrementStateCount(HealthState state) {
    switch (state) {
    case HealthState::NOMINAL:
        ++summary_.nominal_count;
        break;
    case HealthState::WARNING:
        ++summary_.warning_count;
        break;
    case HealthState::CRITICAL:
        ++summary_.critical_count;
        break;
    case HealthState::FAULT:
        ++summary_.fault_count;
        break;
    case HealthState::INIT:
        break;
    }
}