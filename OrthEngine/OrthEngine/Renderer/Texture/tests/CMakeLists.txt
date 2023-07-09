# Create a library target for TextureMock
add_library(TextureFactoryMock INTERFACE TextureFactoryMock.hpp)
target_include_directories(TextureFactoryMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Texture/tests")
target_link_libraries(TextureFactoryMock INTERFACE GTest::gmock Texture)

# Create TextureFactoryTest executable for unit tests
add_executable(TextureFactoryTest
  TextureFactoryTests.cpp)
set_target_properties(TextureFactoryTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(TextureFactoryTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  OpenGLFixture
  Texture
)

target_include_directories(TextureFactoryTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Texture"
)

include(GoogleTest)
gtest_discover_tests(TextureFactoryTest)