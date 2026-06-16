cmake_minimum_required(VERSION 3.16)
project(pypilot_signalk_connector LANGUAGES CXX)

option(PYPILOT_SIGNALK_CONNECTOR_BUILD_TESTS "Build tests" ON)
option(PYPILOT_SIGNALK_CONNECTOR_BUILD_EXAMPLES "Build examples" ON)

set(PYPILOT_DATA_MODEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../pypilot-data-model/src" CACHE PATH "Path to pypilot-data-model src include dir")

add_library(pypilot_signalk_connector INTERFACE)
target_include_directories(pypilot_signalk_connector INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PYPILOT_DATA_MODEL_DIR}
)
target_compile_features(pypilot_signalk_connector INTERFACE cxx_std_11)

if(PYPILOT_SIGNALK_CONNECTOR_BUILD_EXAMPLES)
  add_executable(apply_path_value examples/linux/apply_path_value.cpp)
  target_link_libraries(apply_path_value PRIVATE pypilot_signalk_connector)

  add_executable(format_delta examples/linux/format_delta.cpp)
  target_link_libraries(format_delta PRIVATE pypilot_signalk_connector)
endif()

if(PYPILOT_SIGNALK_CONNECTOR_BUILD_TESTS)
  enable_testing()
  foreach(test_name
      test_apply_wind
      test_apply_gps
      test_apply_rudder
      test_apply_imu
      test_apply_water
      test_apply_put_control
      test_format_delta
      test_subscription)
    add_executable(${test_name} tests/${test_name}.cpp)
    target_link_libraries(${test_name} PRIVATE pypilot_signalk_connector)
    add_test(NAME ${test_name} COMMAND ${test_name})
  endforeach()
endif()
