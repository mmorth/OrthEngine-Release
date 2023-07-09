# Create a library target for WindowMock
add_library(RasterizerMock INTERFACE RasterizerMock.hpp)
target_include_directories(RasterizerMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Rasterizers/tests")
target_link_libraries(RasterizerMock INTERFACE GTest::gmock)

# Create TextureFactoryTest executable for unit tests
add_executable(RasterizerTest
  RasterizerTests.cpp
)
set_target_properties(RasterizerTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(RasterizerTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  OpenGLFixture
  Rasterizer
  ShaderMock
)

target_include_directories(RasterizerTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Rasterizers"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/tests"
)

include(GoogleTest)
gtest_discover_tests(RasterizerTest)