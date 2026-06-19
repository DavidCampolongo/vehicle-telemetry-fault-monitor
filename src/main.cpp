#include <iostream>
#include <vector>
#include <exception>

#include "TelemetryRecord.h"
#include "HealthState.h"
#include "FaultRules.h"

using namespace std;

void printRecordWithState(const TelemetryRecord& record) {
    HealthState state = evaluateHealth(record);

    cout << record.timestamp_ms << " "
         << record.battery_voltage << " "
         << record.temperature_c << " "
         << record.current_draw_a << " "
         << record.link_status << " "
         << record.sensor_valid << " -> "
         << toString(state) << endl;
}

int main() {
    try {
        vector<TelemetryRecord> records =
            ParseTelemetryFile("data/sample_telemetry.csv");

        if (records.size() < 5) {
            cerr << "Not enough telemetry records loaded.\n";
            return 1;
        }

        printRecordWithState(records[0]);
        printRecordWithState(records[4]);
    }
    catch (const exception& error) {
        cerr << "Error: " << error.what() << endl;
        return 1;
    }

    return 0;
}