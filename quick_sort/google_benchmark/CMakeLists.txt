project(google_benchmark)

add_subdirectory(lib)
include_directories(lib/include)

add_executable(google_benchmark_run bench.cpp)

target_link_libraries(google_benchmark_run benchmark::benchmark)