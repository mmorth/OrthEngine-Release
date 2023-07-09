add_executable(GlobalSettingsTest
  GlobalSettingsTests.cpp
)
set_target_properties(GlobalSettingsTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(GlobalSettingsTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  GlobalSettingsConstants
)

target_include_directories(GlobalSettingsTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/GlobalSettings"
)

include(GoogleTest)
gtest_discover_tests(GlobalSettingsTest)