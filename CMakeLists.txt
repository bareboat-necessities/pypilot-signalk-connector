cmake_minimum_required(VERSION 3.16)
project(pypilot_signalk_connector LANGUAGES CXX)

option(PYPILOT_SIGNALK_CONNECTOR_BUILD_TESTS "Build tests" ON)
option(PYPILOT_SIGNALK_CONNECTOR_BUILD_EXAMPLES "Build examples" ON)

set(PYPILOT_DATA_MODEL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../pypilot-data-model/src" CACHE PATH "Path to pypilot-data-model src include dir")
set(PYPILOT_MDNS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../pypilot-mdns" CACHE PATH "Path to pypilot-mdns checkout")

if(EXISTS "${PYPILOT_MDNS_DIR}/CMakeLists.txt")
  add_subdirectory("${PYPILOT_MDNS_DIR}" "${CMAKE_CURRENT_BINARY_DIR}/pypilot-mdns")
  set(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS ON)
else()
  set(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS OFF)
endif()

add_library(pypilot_signalk_connector INTERFACE)
target_include_directories(pypilot_signalk_connector INTERFACE
  ${CMAKE_CURRENT_SOURCE_DIR}/src
  ${PYPILOT_DATA_MODEL_DIR}
)
target_compile_features(pypilot_signalk_connector INTERFACE cxx_std_17)
if(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS)
  target_link_libraries(pypilot_signalk_connector INTERFACE pypilot_mdns)
  target_compile_definitions(pypilot_signalk_connector INTERFACE PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS=1)
endif()

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

  add_executable(test_delta_json tests/test_delta_json.cpp)
  target_link_libraries(test_delta_json PRIVATE pypilot_signalk_connector)
  add_test(NAME test_delta_json COMMAND test_delta_json)

  add_executable(test_transport_and_messages tests/test_transport_and_messages.cpp)
  target_link_libraries(test_transport_and_messages PRIVATE pypilot_signalk_connector)
  add_test(NAME test_transport_and_messages COMMAND test_transport_and_messages)

  add_executable(test_outbound_delta tests/test_outbound_delta.cpp)
  target_link_libraries(test_outbound_delta PRIVATE pypilot_signalk_connector)
  add_test(NAME test_outbound_delta COMMAND test_outbound_delta)

  if(PYPILOT_SIGNALK_CONNECTOR_WITH_MDNS)
    add_executable(test_mdns_discovery_compile tests/test_mdns_discovery_compile.cpp)
    target_link_libraries(test_mdns_discovery_compile PRIVATE pypilot_signalk_connector)
    add_test(NAME test_mdns_discovery_compile COMMAND test_mdns_discovery_compile)
  endif()
endif()
