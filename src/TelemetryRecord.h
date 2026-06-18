#pragma once

#include <string>

struct TelemetryRecord
{
    int timestamp_ms;
    double battery_voltage;
    double temperature_c;
    double current_draw_a;
    std::string link_status;
    bool sensor_valid;
};

TelemetryRecord loadTelemetry(const std::string& filename);