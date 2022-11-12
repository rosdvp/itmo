# 'Google_test' is the subproject name
project(google_tests)

# 'lib' is the folder with Google Test sources
add_subdirectory(lib)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

# 'Google_Tests_run' is the target name
# 'test1.cpp tests2.cpp' are source files with tests
add_executable(google_tests_run
        Dummy.h
        TestInsertSort.cpp
        TestMedian.cpp
        TestSortArrayInt.cpp
        TestSortArrayString.cpp
        TestSortDummies.cpp
        TestSortVectorInt.cpp)

target_link_libraries(google_tests_run gtest gtest_main)