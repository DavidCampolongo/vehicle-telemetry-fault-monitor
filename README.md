# Vehicle Telemetry Fault Monitor

A C++17 command-line telemetry monitor that reads simulated vehicle data from a CSV file, evaluates each record against fault rules, tracks system health, and prints a summary report.

I built this project because I am interested in the kind of telemetry and fault-monitoring systems used in high-performance engineering environments. I am especially passionate about Formula 1, where teams depend on fast, reliable telemetry to understand car behavior, detect problems, and make decisions under pressure. This project is a small software model of that idea: raw vehicle data comes in, the system validates it, classifies health states, records important events, and produces a readable report.

## What This Project Demonstrates

* C++17 programming
* Modular source/header organization
* CSV parsing and input validation
* Error handling with exceptions
* Rule-based fault detection
* State tracking and severity ranking
* Basic test coverage using `assert`
* Separation between parsing, evaluation, monitoring, and reporting

## System Overview

```text
CSV telemetry file
        |
        v
Telemetry parser
        |
        v
Fault rule engine
        |
        v
Fault monitor / state tracker
        |
        v
Console report
```

The program is intentionally split into small components:

* `TelemetryRecord` parses and stores one row of telemetry data.
* `FaultRules` evaluates one telemetry record and returns a health state plus a reason.
* `FaultMonitor` processes records, counts states, tracks the worst state, and records events.
* `ReportWriter` formats the final report.

## Telemetry Input Format

The input file must use this CSV header:

```csv
timestamp_ms,battery_voltage,temperature_c,current_draw_a,link_status,sensor_valid
```

Example input:

```csv
timestamp_ms,battery_voltage,temperature_c,current_draw_a,link_status,sensor_valid
0,12.6,35.0,4.2,true,true
1000,13.0,40.0,4.5,true,true
2000,10.5,78.0,5.0,true,true
3000,12.1,92.0,7.5,true,true
4000,12.0,45.0,4.0,false,true
5000,12.4,41.0,4.1,true,false
```

## Health States

| State      | Meaning                                              |
| ---------- | ---------------------------------------------------- |
| `INIT`     | Initial state before telemetry is processed          |
| `NOMINAL`  | Telemetry values are within normal operating bounds  |
| `WARNING`  | One or more values are near unsafe limits            |
| `CRITICAL` | One or more values exceed critical limits            |
| `FAULT`    | Sensor data is invalid or communication link is lost |

## Fault Rules

The monitor evaluates each telemetry record using this severity order:

```text
FAULT > CRITICAL > WARNING > NOMINAL
```

| Condition                                | Resulting State |
| ---------------------------------------- | --------------- |
| `sensor_valid == false`                  | `FAULT`         |
| `link_status == false`                   | `FAULT`         |
| `battery_voltage < 10.0`                 | `CRITICAL`      |
| `temperature_c > 90.0`                   | `CRITICAL`      |
| `current_draw_a > 12.0`                  | `CRITICAL`      |
| `10.0 <= battery_voltage <= 12.0`        | `WARNING`       |
| `75.0 <= temperature_c <= 90.0`          | `WARNING`       |
| `current_draw_a > 8.0`                   | `WARNING`       |
| No fault, critical, or warning condition | `NOMINAL`       |

If multiple conditions are found in one record, the most severe state wins, while the report keeps the relevant reasons.

## Project Structure

```text
data/
  sample_telemetry.csv

src/
  FaultEvent.h
  FaultMonitor.cpp
  FaultMonitor.h
  FaultRules.cpp
  FaultRules.h
  HealthState.cpp
  HealthState.h
  ReportWriter.cpp
  ReportWriter.h
  TelemetryRecord.cpp
  TelemetryRecord.h
  main.cpp

tests/
  run_tests.cpp

CMakeLists.txt
LICENSE
README.md
```

## Build With CMake

This project builds on Windows, macOS, and Linux using CMake.

### Windows

From PowerShell:

```powershell
cmake -S . -B build
cmake --build build --config Debug
```

Run the tests:

```powershell
ctest --test-dir build --build-config Debug
```

Run the program:

```powershell
.\build\Debug\telemetry_monitor.exe
```

Run with an explicit CSV file:

```powershell
.\build\Debug\telemetry_monitor.exe data\sample_telemetry.csv
```

### macOS / Linux

From a terminal:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```

Run the tests:

```bash
ctest --test-dir build
```

Run the program:

```bash
./build/telemetry_monitor
```

Run with an explicit CSV file:

```bash
./build/telemetry_monitor data/sample_telemetry.csv
```

## Example Output

```text
Telemetry Monitor Report
========================

Records processed: 6

State counts:
  NOMINAL:  2
  WARNING:  1
  CRITICAL: 1
  FAULT:    2

Worst state observed: FAULT

Events:
  [2000 ms] NOMINAL -> WARNING: battery voltage in warning range; temperature in warning range
  [3000 ms] WARNING -> CRITICAL: temperature above critical threshold
  [4000 ms] CRITICAL -> FAULT: communication link lost; battery voltage in warning range
  [5000 ms] FAULT -> FAULT: sensor invalid
```

## Tests

The test runner covers:

* Valid telemetry parsing
* Rejection of invalid numeric fields
* Rejection of malformed CSV rows
* Health-state evaluation
* Multiple warning reasons
* Critical conditions overriding warnings
* Fault monitor summary counts and event tracking

The tests use standard C++ `assert`, so a successful run should complete without assertion failures.

## Future Improvements

Possible next steps:

* Add a formal unit testing framework
* Add configurable thresholds through a config file
* Export reports to a text file or JSON
* Add timestamp ordering validation
* Add more vehicle telemetry channels
* Add a live/streaming telemetry mode
* Add a simple dashboard-style visualization layer
* Expand this into an embedded-style monitor with memory-mapped status registers

## License

This project is licensed under the MIT License.