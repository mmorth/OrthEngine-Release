# Create a library target for WindowMock
add_library(FactoryMock INTERFACE FactoryMock.hpp)
target_include_directories(FactoryMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Factories/tests")
target_link_libraries(FactoryMock INTERFACE GTest::gmock Factories)


# Create executable for FactoryTest
add_executable(FactoryTest
  FactoryTests.cpp)
set_target_properties(FactoryTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(FactoryTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  MathUtils
  OpenGLFixture
  Camera
  CameraMock
  Shader
  Factories
  FactoryMock
  Framebuffer
  FramebufferMock
  Publisher
  ShaderMock
  RasterizerMock
  RenderObjectMock
  Texture
  TextureFactoryMock
)

target_include_directories(FactoryTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Rasterizers/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/RenderObjects/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Texture/tests"
)

include(GoogleTest)
gtest_discover_tests(FactoryTest)