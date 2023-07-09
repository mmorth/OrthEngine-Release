# Create a library target for WindowMock
add_library(CameraMock INTERFACE CameraMock.hpp)
target_include_directories(CameraMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests")
target_link_libraries(CameraMock INTERFACE GTest::gmock Camera)

# Create TextureFactoryTest executable for unit tests
add_executable(CameraTest
  CameraTests.cpp
)
set_target_properties(CameraTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(CameraTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  OpenGLFixture
  Camera
)

target_include_directories(CameraTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera"
)

include(GoogleTest)
gtest_discover_tests(CameraTest)