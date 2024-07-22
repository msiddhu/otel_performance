cmake_minimum_required(VERSION 3.10)
project(otel_performance)

# Enable C++17
set(CMAKE_CXX_STANDARD 17)

# Find OpenTelemetry
find_package(opentelemetry-cpp CONFIG REQUIRED)

# Find Google Benchmark
find_package(benchmark CONFIG REQUIRED)

# Find Protobuf
find_package(Protobuf REQUIRED)

# Include directories
include_directories(${Protobuf_INCLUDE_DIRS})

include_directories(${OPENTELEMETRY_CPP_INCLUDE_DIRS})


add_executable(otel_performance main.cpp tracer_common.h)
target_link_libraries(otel_performance PRIVATE
        opentelemetry_trace
        opentelemetry_exporter_otlp_http
        opentelemetry_otlp_recordable
        opentelemetry_proto
        benchmark
        ${Protobuf_LIBRARIES}
)
