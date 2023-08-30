# Create a library target for WindowMock
add_library(PhysicsWorldMock INTERFACE PhysicsWorldMock.hpp)
target_include_directories(PhysicsWorldMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsWorld/tests")
target_link_libraries(PhysicsWorldMock INTERFACE GTest::gmock PhysicsWorld)


# Create executable for PhysicsWorldTest
add_executable(PhysicsWorldTest
  PhysicsWorldTests.cpp)
set_target_properties(PhysicsWorldTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(PhysicsWorldTest
  GTest::gtest
  GTest::gtest_main
  PhysicsWorld
)

target_include_directories(PhysicsWorldTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsWorld"
)

include(GoogleTest)
gtest_discover_tests(PhysicsWorldTest)