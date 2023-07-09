# Create a library target for WindowMock
add_library(WindowMock INTERFACE WindowMock.hpp)
target_include_directories(WindowMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Window/tests")
target_link_libraries(WindowMock INTERFACE GTest::gmock Window)


# Create WindowTest executable for unit tests
add_executable(WindowTest
  WindowTests.cpp
)
set_target_properties(WindowTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(WindowTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  Publisher
  PublisherMock
  Window
)

target_include_directories(WindowTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Window"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application/EventSystem/tests"
)

include(GoogleTest)
gtest_discover_tests(WindowTest)