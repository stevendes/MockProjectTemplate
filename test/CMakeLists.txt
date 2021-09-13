cmake_minimum_required(VERSION 3.10.2)

set(PROJECT_TEST_SRCS sensor_receiver_tests.cpp sensor_zone_tests.cpp memory_tests.cpp ux_tests.cpp manager_tests)

include(GoogleTest)
include(CTest)

add_executable(${PROJECT_NAME}_test ${PROJECT_TEST_SRCS})

target_link_libraries(${PROJECT_NAME}_test gtest gmock gtest_main
                      ${PROJECT_NAME})

gtest_discover_tests(${PROJECT_NAME}_test)
