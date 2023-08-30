# Create executable for PhysicsUtilsTest
add_executable(PhysicsUtilsTest
  PhysicsUtilsTests.cpp)
set_target_properties(PhysicsUtilsTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(PhysicsUtilsTest
  GTest::gtest
  GTest::gtest_main
  PhysicsUtils
)

target_include_directories(PhysicsUtilsTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsUtils"
)

include(GoogleTest)
gtest_discover_tests(PhysicsUtilsTest)