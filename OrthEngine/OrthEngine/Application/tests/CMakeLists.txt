# ===== Create executable for ApplicationTest =====
add_executable(ApplicationTest
  ApplicationTests.cpp)
set_target_properties(ApplicationTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(ApplicationTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  Camera
  CameraMock
  EngineApp
  Factories
  FactoryMock
  Managers
  ManagerMock
  OpenGLFixture
  PublisherMock
  Window
  WindowMock
)

target_include_directories(ApplicationTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application/EventSystem/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Window/tests"
)

include(GoogleTest)
gtest_discover_tests(ApplicationTest)


# ===== Create executable for IntegrationTest =====
add_executable(IntegrationTest
  IntegrationTests.cpp)
set_target_properties(IntegrationTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(IntegrationTest
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  Camera
  CameraMock
  EngineApp
  Factories
  FactoryMock
  Managers
  ManagerMock
  OpenGLFixture
  PublisherMock
  Window
  WindowMock
)

target_include_directories(IntegrationTest PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application/EventSystem/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Window/tests"
)

include(GoogleTest)
gtest_discover_tests(IntegrationTest)