#include "TelemetryRecord.h"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

string getNextField(istream& inputStream) {
    string next;

    if (!getline(inputStream, next, ',')) {
        throw runtime_error("Missing CSV field");
    }

    if (!next.empty() && next.back() == '\r') {
        next.pop_back();
    }

    return next;
}

bool parseBoolField(const string& field) {
    if (field == "true") {
        return true;
    }

    if (field == "false") {
        return false;
    }

    throw runtime_error("Invalid boolean field: " + field);
}

TelemetryRecord ParseTelemetryLine(const string& line) {
    stringstream ss(line);

    TelemetryRecord currentRecord {};

    currentRecord.timestamp_ms = stoi(getNextField(ss));
    currentRecord.battery_voltage = stod(getNextField(ss));
    currentRecord.temperature_c = stod(getNextField(ss));
    currentRecord.current_draw_a = stod(getNextField(ss));
    currentRecord.link_status = parseBoolField(getNextField(ss));
    currentRecord.sensor_valid = parseBoolField(getNextField(ss));

    return currentRecord;
}

vector<TelemetryRecord> ParseTelemetryFile(const string& filename) {
    ifstream telemetryReport(filename);

    if (!telemetryReport) {
        throw runtime_error("Failed to open the telemetry file: " + filename);
    }

    vector<TelemetryRecord> records;
    string line;
    int lineNumber = 0;

    // Skip header line
    if (!getline(telemetryReport, line)) {
        throw runtime_error("Telemetry file is empty: " + filename);
    }

    ++lineNumber;

    while (getline(telemetryReport, line)) {
        ++lineNumber;

        if (line.empty()) {
            continue;
        }

        try {
            TelemetryRecord record = ParseTelemetryLine(line);
            records.push_back(record);
        }
        catch (const exception& error) {
            throw runtime_error(
                "Failed to parse telemetry file at line " +
                to_string(lineNumber) +
                ": " +
                error.what()
            );
        }
    }

    return records;
}