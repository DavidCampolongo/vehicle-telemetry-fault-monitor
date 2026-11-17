CXX ?= c++
CXXFLAGS ?= -std=c++17 -Wall -Wextra -Wpedantic
BUILD_DIR = build
TARGET = $(BUILD_DIR)/telemetry-monitor
TEST_TARGET = $(BUILD_DIR)/test-telemetry
SOURCES = $(wildcard src/*.cpp)
CORE = src/TelemetryRecord.cpp src/HealthState.cpp src/FaultRules.cpp src/FaultMonitor.cpp
HEADERS = $(wildcard src/*.h)

all: $(TARGET)

$(TARGET): $(SOURCES) $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $@

$(TEST_TARGET): tests/run_tests.cpp $(CORE) $(HEADERS) | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -I src tests/run_tests.cpp $(CORE) -o $@

$(BUILD_DIR):
	mkdir -p $@

test: $(TEST_TARGET)
	$(TEST_TARGET)

demo: $(TARGET)
	$(TARGET) data/sample_telemetry.csv

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all test demo clean
