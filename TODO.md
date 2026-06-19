# TODO

## 1. Add Fault Event Logging

* Create a `FaultEvent` structure
* Store:
  * timestamp
  * severity
  * diagnostic message
* Generate fault events from telemetry records when warning, critical, or fault conditions are detected
* Store events in `std::vector<FaultEvent>`

### 3. Track State Transitions

* Track the previous health state and the new health state
* Record transitions when the state changes
* Example:

  * `INIT -> NOMINAL`
  * `NOMINAL -> WARNING`
  * `WARNING -> CRITICAL`
  * `CRITICAL -> FAULT`

### 4. Build Basic Monitor Flow

* Process all telemetry records in order
* Evaluate health state for each record
* Log fault events
* Count processed samples
* Count invalid samples
* Track final health state

### 5. Generate Console Report

### 6. Write Report to File

* Create a `reports/` folder
* Write the final report to a `.txt` file with current date / hour / min / s timestamp

### 7. Deal with some error handling like

* Valid telemetry row is accepted
* Missing fields are rejected
* Non-numerics are rejected
* Header row is skipped correctly

### 8. README Polish

Update the README with:

* Build instructions
* Run instructions
* Sample input
* Sample output
* Project architecture
* Fault rule table
* Current limitations