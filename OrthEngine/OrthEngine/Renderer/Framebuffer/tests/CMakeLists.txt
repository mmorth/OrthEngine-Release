# Create a library target for WindowMock
add_library(FramebufferMock INTERFACE FramebufferMock.hpp)
target_include_directories(FramebufferMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Framebuffer/tests")
target_link_libraries(FramebufferMock INTERFACE GTest::gmock Framebuffer)

# Create TextureFactoryTest executable for unit tests
add_executable(FramebufferTest
  FramebufferTests.cpp
)
set_target_properties(FramebufferTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(FramebufferTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  GlobalSettingsConstants
  OpenGLFixture
  ShaderMock
  Framebuffer
)

target_include_directories(FramebufferTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Framebuffer"
)

include(GoogleTest)
gtest_discover_tests(FramebufferTest)