# Create a library target for WindowMock
add_library(RenderObjectMock INTERFACE RenderObjectMock.hpp)
target_include_directories(RenderObjectMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Data/RenderObjects/tests")
target_link_libraries(RenderObjectMock INTERFACE GTest::gmock RenderObjects)

# Create TextureFactoryTest executable for unit tests
add_executable(RenderObjectTest
  RenderObjectTests.cpp
)
set_target_properties(RenderObjectTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(RenderObjectTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  OpenGLFixture
  ShaderMock
  TransformMock
  RenderObjects
)

target_include_directories(RenderObjectTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/RenderObjects"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Transform/tests"
)

include(GoogleTest)
gtest_discover_tests(RenderObjectTest)