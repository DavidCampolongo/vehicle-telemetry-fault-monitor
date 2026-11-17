# Vehicle Telemetry Fault Monitor

[![Build and test](https://github.com/DavidCampolongo/vehicle-telemetry-fault-monitor/actions/workflows/build.yml/badge.svg)](https://github.com/DavidCampolongo/vehicle-telemetry-fault-monitor/actions/workflows/build.yml)

A C++17 program that reads simulated vehicle telemetry from CSV, classifies each record, and reports faults and state transitions. It separates parsing, classification, state tracking, and reporting so each part can be tested independently.

## Run it

Requires a C++17 compiler and Make. No external libraries.

```sh
make
make test
make demo
# Or provide another CSV:
./build/telemetry-monitor data/sample_telemetry.csv
```

The included six-record sample produces two nominal records, one warning, one critical record, and two faults. The report includes timestamps and reasons for transitions, with `FAULT` as the worst state.

## Input and behavior

```csv
timestamp_ms,battery_voltage,temperature_c,current_draw_a,link_status,sensor_valid
0,12.6,35.0,4.2,true,true
```

| Condition | Classification |
| --- | --- |
| Invalid sensor or lost communication link | Fault |
| Battery below 10 V | Critical |
| Battery from 10 through 12 V | Warning |
| Temperature above 90 °C | Critical |
| Temperature from 75 through 90 °C | Warning |
| Current above 12 A | Critical |
| Current above 8 through 12 A | Warning |

The highest applicable severity wins: **Fault > Critical > Warning > Nominal**. Multiple findings remain in the reason text. Invalid sensor data is not evaluated against numeric thresholds. Malformed CSV input stops processing with an error and a nonzero exit status.

## Design and tests

- [TelemetryRecord](src/TelemetryRecord.cpp): parse and validate CSV fields.
- [FaultRules](src/FaultRules.cpp): evaluate thresholds and retain the reasons for a classification.
- [FaultMonitor](src/FaultMonitor.cpp): count states and record transitions/fault events.
- [ReportWriter](src/ReportWriter.cpp): format the summary without changing classification logic.

[Tests](tests/run_tests.cpp) cover parsing, malformed fields, severity precedence, combined reasons, and summary/event counts. GitHub Actions builds and tests on Linux and macOS for pushes and pull requests.

## Scope

This is a host-based simulation using sample data. It does not connect to vehicle hardware, implement an RTOS, or provide safety-certified diagnostics. Thresholds are illustrative and configured in `FaultRules.cpp`.
