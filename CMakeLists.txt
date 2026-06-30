cmake_minimum_required(VERSION 3.16)

project(
    VehicleTelemetryFaultMonitor
    VERSION 1.0.0
    DESCRIPTION "C++ vehicle telemetry fault monitor"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(TELEMETRY_CORE_SOURCES
    src/TelemetryRecord.cpp
    src/HealthState.cpp
    src/FaultRules.cpp
    src/FaultMonitor.cpp
)

add_library(telemetry_core STATIC ${TELEMETRY_CORE_SOURCES})
target_include_directories(telemetry_core PUBLIC src)

add_executable(telemetry_monitor
    src/main.cpp
    src/ReportWriter.cpp
)
target_link_libraries(telemetry_monitor PRIVATE telemetry_core)

if(MSVC)
    target_compile_options(telemetry_core PRIVATE /W4 /permissive-)
    target_compile_options(telemetry_monitor PRIVATE /W4 /permissive-)
else()
    target_compile_options(telemetry_core PRIVATE -Wall -Wextra -Wpedantic)
    target_compile_options(telemetry_monitor PRIVATE -Wall -Wextra -Wpedantic)
endif()

file(COPY data DESTINATION ${CMAKE_BINARY_DIR})

enable_testing()

add_executable(telemetry_tests
    tests/run_tests.cpp
)
target_link_libraries(telemetry_tests PRIVATE telemetry_core)

if(MSVC)
    target_compile_options(telemetry_tests PRIVATE /W4 /permissive-)
else()
    target_compile_options(telemetry_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_test(NAME telemetry_tests COMMAND telemetry_tests)
