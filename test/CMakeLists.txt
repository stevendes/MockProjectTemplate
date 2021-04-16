cmake_minimum_required(VERSION 3.10.2)

set(PROJECT_TEST_SRCS runner.cpp test_cases.cpp)

include(GoogleTest)
include(CTest)

add_executable(${PROJECT_NAME}_test ${PROJECT_TEST_SRCS})

target_link_libraries(${PROJECT_NAME}_test gtest gmock gtest_main
                      ${PROJECT_NAME})

gtest_discover_tests(${PROJECT_NAME}_test)
