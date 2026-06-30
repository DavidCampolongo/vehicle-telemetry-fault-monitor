#include "ReportWriter.h"

#include "HealthState.h"

void printReport(const MonitorSummary& summary, std::ostream& output) {
    output << "Telemetry Monitor Report\n";
    output << "========================\n\n";

    output << "Records processed: " << summary.records_processed << "\n\n";

    output << "State counts:\n";
    output << "  NOMINAL:  " << summary.nominal_count << "\n";
    output << "  WARNING:  " << summary.warning_count << "\n";
    output << "  CRITICAL: " << summary.critical_count << "\n";
    output << "  FAULT:    " << summary.fault_count << "\n\n";

    output << "Worst state observed: " << toString(summary.worst_state) << "\n\n";

    output << "Events:\n";

    if (summary.events.empty()) {
        output << "  No warning, critical, or fault events recorded.\n";
        return;
    }

    for (const FaultEvent& event : summary.events) {
        output << "  ["
               << event.timestamp_ms
               << " ms] "
               << toString(event.previous_state)
               << " -> "
               << toString(event.current_state)
               << ": "
               << event.reason
               << "\n";
    }
}