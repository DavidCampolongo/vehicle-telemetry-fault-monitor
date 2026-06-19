#pragma once

#include <string>
#include <vector>

struct TelemetryRecord {
    int timestamp_ms;
    double battery_voltage;
    double temperature_c;
    double current_draw_a;
    bool link_status;
    bool sensor_valid;
};

TelemetryRecord ParseTelemetryLine(const std::string& line);

std::vector<TelemetryRecord> ParseTelemetryFile(const std::string& filename);