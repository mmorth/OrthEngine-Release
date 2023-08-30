# Create a library target for WindowMock
add_library(RigidBodyMock INTERFACE RigidBodyMock.hpp)
target_include_directories(RigidBodyMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Physics/RigidBody/tests")
target_link_libraries(RigidBodyMock INTERFACE GTest::gmock RigidBody)


# Create executable for RigidBodyTest
add_executable(RigidBodyTest
  RigidBodyTests.cpp)
set_target_properties(RigidBodyTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(RigidBodyTest
  GTest::gtest
  GTest::gtest_main
  RigidBody
)

target_include_directories(RigidBodyTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Physics/RigidBody"
)

include(GoogleTest)
gtest_discover_tests(RigidBodyTest)