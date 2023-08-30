# TODO: Resovle issue with PhysicsManager requiring to bring in all external dependencies
add_library(EngineApp STATIC EngineApp.cpp EngineApp.hpp)
target_include_directories(EngineApp 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsUtils
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/Callbacks
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsWorld
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/RigidBody
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories)
target_link_libraries(EngineApp PRIVATE 
	Camera
	Factories
	Managers
	PhysicsWorld
	Texture
	Transform
	Window
	${G3LOG_LIBRARIES}
)

add_subdirectory(EventSystem)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
