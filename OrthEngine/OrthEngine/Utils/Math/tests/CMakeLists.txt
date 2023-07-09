cmake_minimum_required(VERSION 3.17)

add_executable(MathUtilsTest
  MathUtilsTests.cpp
)

target_link_libraries(MathUtilsTest
  GTest::gtest_main
  MathUtils
)

target_include_directories(MathUtilsTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math"
)

include(GoogleTest)
gtest_discover_tests(MathUtilsTest)