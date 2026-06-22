#include "TelemetryRecord.h"

#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

namespace {
const string EXPECTED_HEADER =
    "timestamp_ms,battery_voltage,temperature_c,current_draw_a,link_status,sensor_valid";

void removeTrailingCarriageReturn(string& text) {
    if (!text.empty() && text.back() == '\r') {
        text.pop_back();
    }
}

void validateFieldCount(const string& line) {
    const int EXPECTED_COMMAS = 5;
    int commaCount = static_cast<int>(count(line.begin(), line.end(), ','));

    if (commaCount != EXPECTED_COMMAS) {
        throw runtime_error("Expected 6 CSV fields");
    }
}

string getNextField(istream& inputStream, const string& fieldName) {
    string next;

    if (!getline(inputStream, next, ',')) {
        throw runtime_error("Missing CSV field: " + fieldName);
    }

    removeTrailingCarriageReturn(next);
    return next;
}

int parseIntField(const string& field, const string& fieldName) {
    size_t parsedCharacters = 0;

    try {
        int value = stoi(field, &parsedCharacters);

        if (parsedCharacters != field.size()) {
            throw runtime_error("Invalid integer field: " + fieldName);
        }

        return value;
    }
    catch (const invalid_argument&) {
        throw runtime_error("Invalid integer field: " + fieldName);
    }
    catch (const out_of_range&) {
        throw runtime_error("Integer field out of range: " + fieldName);
    }
}

double parseDoubleField(const string& field, const string& fieldName) {
    size_t parsedCharacters = 0;

    try {
        double value = stod(field, &parsedCharacters);

        if (parsedCharacters != field.size()) {
            throw runtime_error("Invalid numeric field: " + fieldName);
        }

        return value;
    }
    catch (const invalid_argument&) {
        throw runtime_error("Invalid numeric field: " + fieldName);
    }
    catch (const out_of_range&) {
        throw runtime_error("Numeric field out of range: " + fieldName);
    }
}

bool parseBoolField(const string& field, const string& fieldName) {
    if (field == "true") {
        return true;
    }

    if (field == "false") {
        return false;
    }

    throw runtime_error("Invalid boolean field: " + fieldName);
}
}

TelemetryRecord ParseTelemetryLine(const string& line) {
    validateFieldCount(line);

    stringstream ss(line);

    TelemetryRecord currentRecord {};

    currentRecord.timestamp_ms =
        parseIntField(getNextField(ss, "timestamp_ms"), "timestamp_ms");

    currentRecord.battery_voltage =
        parseDoubleField(getNextField(ss, "battery_voltage"), "battery_voltage");

    currentRecord.temperature_c =
        parseDoubleField(getNextField(ss, "temperature_c"), "temperature_c");

    currentRecord.current_draw_a =
        parseDoubleField(getNextField(ss, "current_draw_a"), "current_draw_a");

    currentRecord.link_ok =
        parseBoolField(getNextField(ss, "link_status"), "link_status");

    currentRecord.sensor_valid =
        parseBoolField(getNextField(ss, "sensor_valid"), "sensor_valid");

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

    if (!getline(telemetryReport, line)) {
        throw runtime_error("Telemetry file is empty: " + filename);
    }

    ++lineNumber;
    removeTrailingCarriageReturn(line);

    if (line != EXPECTED_HEADER) {
        throw runtime_error("Unexpected CSV header in file: " + filename);
    }

    while (getline(telemetryReport, line)) {
        ++lineNumber;

        removeTrailingCarriageReturn(line);

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