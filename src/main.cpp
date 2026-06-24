#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "FaultMonitor.h"
#include "ReportWriter.h"
#include "TelemetryRecord.h"

int main(int argc, char* argv[]) {
    const std::string filename =
        argc > 1 ? argv[1] : "data/sample_telemetry.csv";

    try {
        std::vector<TelemetryRecord> records = ParseTelemetryFile(filename);

        FaultMonitor monitor;
        monitor.processRecords(records);

        printReport(monitor.getSummary(), std::cout);
    }
    catch (const std::exception& error) {
        std::cerr << "Error: " << error.what() << '\n';
        return 1;
    }

    return 0;
}