message("Building TestRunner")

add_executable(TestRunner
    TestRunner.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Application/tests/ApplicationTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Application/EventSystem/tests/PublisherTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories/tests/FactoryTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Data/GlobalSettingsConstants/tests/GlobalSettingsTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers/tests/ManagerTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Data/RenderObjects/tests/RenderObjectTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsWorld/tests/PhysicsWorldTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Physics/RigidBody/tests/RigidBodyTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests/CameraTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Framebuffer/tests/FramebufferTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Rasterizers/tests/RasterizerTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/tests/ShaderTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Texture/tests/TextureFactoryTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Transform/tests/TransformTests.cpp
    ${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Window/tests/WindowTests.cpp
)
set_target_properties(TestRunner PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

target_link_libraries(TestRunner
    GTest::gtest
    GTest::gtest_main
    GTest::gmock
    GTest::gmock_main
    Camera
    EngineApp
    Factories
    FactoryMock
    Framebuffer
    GlobalSettingsConstants
    Managers
    ManagerMock
    OpenGLFixture
    PhysicsWorldMock
    Publisher
    PublisherMock
    Rasterizer
    RenderObjects
    RenderObjectMock
    RigidBody
    Shader
    Texture
    TextureFactoryMock
    Transform
    Window
    ${G3LOG_LIBRARIES}
)

target_include_directories(TestRunner PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Application/EventSystem/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Data/RenderObjects/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsWorld/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Camera/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Texture/tests"
    "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/tests"
)

include(GoogleTest)
gtest_discover_tests(TestRunner)

add_subdirectory(Fixtures)
