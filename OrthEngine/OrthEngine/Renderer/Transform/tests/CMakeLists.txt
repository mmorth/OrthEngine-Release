# Create a library target for WindowMock
add_library(TransformMock INTERFACE TransformMock.hpp)
target_include_directories(TransformMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Transform/tests")
target_link_libraries(TransformMock INTERFACE GTest::gmock Transform)


# Create executable for TransformTest
add_executable(TransformTest
  TransformTests.cpp)
set_target_properties(TransformTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(TransformTest
  GTest::gtest
  GTest::gtest_main
  Transform
)

target_include_directories(TransformTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Transform"
)

include(GoogleTest)
gtest_discover_tests(TransformTest)