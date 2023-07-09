# Create a library target for PublisherMock
add_library(PublisherMock INTERFACE PublisherMock.hpp)
target_include_directories(PublisherMock INTERFACE "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/EventSystem/tests")
target_link_libraries(PublisherMock INTERFACE GTest::gmock Publisher)


add_executable(PublisherTest
  PublisherTests.cpp
)
set_target_properties(PublisherTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(PublisherTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  Publisher
)

target_include_directories(PublisherTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Publisher"
)

include(GoogleTest)
gtest_discover_tests(PublisherTest)