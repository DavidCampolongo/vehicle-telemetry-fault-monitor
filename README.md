# Vehicle Telemetry Fault Monitor

A C++ host-based telemetry fault monitor that reads simulated vehicle telemetry from a CSV file, evaluates system health, records warning/critical/fault events, and prints a summary report.

## Features

* Reads telemetry records from a CSV file
* Validates CSV header and field count
* Parses numeric and boolean telemetry fields
* Evaluates system health using fault rules
* Tracks health state counts
* Tracks the worst state observed
* Records warning, critical, and fault events
* Prints a clean telemetry report
* Includes basic tests using `assert`

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

The monitor evaluates each telemetry record using the following priority:

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

.gitignore
LICENSE
README.md
```

## Build

Compile the telemetry monitor with:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic src/*.cpp -o telemetry_monitor.out
```

## Run

Run using the default sample telemetry file:

```bash
./telemetry_monitor.out
```

Or provide a CSV file path manually:

```bash
./telemetry_monitor.out data/sample_telemetry.csv
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
  [2000 ms] NOMINAL -> WARNING: battery voltage in warning range
  [3000 ms] WARNING -> CRITICAL: temperature above critical threshold
  [4000 ms] CRITICAL -> FAULT: communication link lost
  [5000 ms] FAULT -> FAULT: sensor invalid
```

## Run Tests

Compile the test runner:

```bash
g++ -std=c++17 -Wall -Wextra -Wpedantic -I src \
    tests/run_tests.cpp \
    src/TelemetryRecord.cpp \
    src/HealthState.cpp \
    src/FaultRules.cpp \
    src/FaultMonitor.cpp \
    -o tests_runner.out
```

Run the tests:

```bash
./tests_runner.out
```

The test runner uses `assert`, so no output means all tests passed.

## Future Improvements

Possible future improvements include:

* Add CMake support
* Add a formal unit testing framework
* Export reports to a text file
* Add configurable threshold values
* Add more telemetry fields
* Add timestamp ordering validation
* Add JSON or config-file support
* Add continuous stream processing mode

## License

This project is licensed under the MIT License.