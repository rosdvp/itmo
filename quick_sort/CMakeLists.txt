cmake_minimum_required(VERSION 3.23)
project(QuickSort)

set(CMAKE_CXX_STANDARD 14)

add_executable(main_target src/QuickSort.h src/TestHelpers.h src/empty.cpp)

add_subdirectory(google_tests)
add_subdirectory(google_benchmark)