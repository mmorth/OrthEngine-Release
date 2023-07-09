# Create a library target for WindowMock
add_library(ShaderMock INTERFACE ShaderMock.hpp)
target_include_directories(ShaderMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/tests")
target_link_libraries(ShaderMock INTERFACE GTest::gmock Shader)


# Create TextureFactoryTest executable for unit tests
add_executable(ShaderTest
  ShaderTests.cpp
)
set_target_properties(ShaderTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(ShaderTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  OpenGLFixture
  Shader
)

target_include_directories(ShaderTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader"
)

include(GoogleTest)
gtest_discover_tests(ShaderTest)