#include <iostream>

#include "TelemetryRecord.h"
#include "HealthState.h"
#include "FaultRules.h"

using namespace std;

int main() {
    TelemetryRecord test = loadTelemetry("src/BaseTest.csv");

    HealthState state = evaluateHealth(test);

    cout << test.timestamp_ms << " "
         << test.battery_voltage << " "
         << test.temperature_c << " "
         << test.current_draw_a << " "
         << test.link_status << " "
         << test.sensor_valid << " -> "
         << toString(state) << endl;

    return 0;
}