#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

using namespace std;

struct TelemetryRecord
{
    int timestamp_ms;
    double battery_voltage;
    double temperature_c;
    double current_draw_a;
    string link_status;
    bool sensor_valid;
};

string getNextField(istream& fileStream) {
    string next;
    getline(fileStream, next, ',');
    return next;
}

TelemetryRecord loadTelemetry (string filename) {
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

    if (!sensorValidText.empty() && sensorValidText.back() == '\r')
    {
        sensorValidText.pop_back();
    }

    currentRecord.sensor_valid = (getNextField(telemetryReport) == "true");

    return currentRecord;
}


int main()
{
    TelemetryRecord test = loadTelemetry("baseTest.csv");

    cout << test.timestamp_ms << " "
         << test.battery_voltage << " "
         << test.temperature_c << " "
         << test.current_draw_a << " "
         << test.link_status << " "
         << test.sensor_valid;

    return 0;
}