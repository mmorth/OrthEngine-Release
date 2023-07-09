# Create a library target for WindowMock
add_library(ManagerMock INTERFACE ManagerMock.hpp)
target_include_directories(ManagerMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers/tests")
target_link_libraries(ManagerMock INTERFACE GTest::gmock Managers)


# Create executable for ManagerTest
add_executable(ManagerTest
  ManagerTests.cpp)
set_target_properties(ManagerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(ManagerTest
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
  ManagerMock
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

target_include_directories(ManagerTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Rasterizers/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/RenderObjects/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Texture/tests"
)

include(GoogleTest)
gtest_discover_tests(ManagerTest)