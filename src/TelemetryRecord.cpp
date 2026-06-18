#include "TelemetryRecord.h"

#include <fstream>
#include <stdexcept>

using namespace std;

string getNextField(istream& fileStream) {
    string next;
    getline(fileStream, next, ',');
    return next;
}

TelemetryRecord loadTelemetry(const string& filename) {
    TelemetryRecord currentRecord;
    ifstream telemetryReport(filename);

    if (!telemetryReport) {
        throw runtime_error("Could not open file: " + filename);
    }

    currentRecord.timestamp_ms = stoi(getNextField(telemetryReport));
    currentRecord.battery_voltage = stod(getNextField(telemetryReport));
    currentRecord.temperature_c = stod(getNextField(telemetryReport));
    currentRecord.current_draw_a = stod(getNextField(telemetryReport));
    currentRecord.link_status = getNextField(telemetryReport);

    string sensorValidText;
    getline(telemetryReport, sensorValidText);

    if (!sensorValidText.empty() && sensorValidText.back() == '\r') {
        sensorValidText.pop_back();
    }

    currentRecord.sensor_valid = (sensorValidText == "true");

    return currentRecord;
}