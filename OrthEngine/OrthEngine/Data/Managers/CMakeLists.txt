add_library(Managers STATIC 
	RenderObjectSceneCreator.cpp RenderObjectSceneCreator.hpp 
	RenderObjectManager.cpp RenderObjectManager.hpp
	 
	Collision/PhysicsManager.hpp Collision/PhysicsManager.cpp)
target_include_directories(Managers 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers
		${CMAKE_SOURCE_DIR}/OrthEngine/
		${CMAKE_SOURCE_DIR}/OrthEngine/Data
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsUtils
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/Callbacks
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/PhysicsWorld
		${CMAKE_SOURCE_DIR}/OrthEngine/Physics/RigidBody
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories
)
target_link_libraries(Managers PRIVATE 
	Camera
	Factories
	Framebuffer
	GlobalSettingsConstants
	ObjectUtils
	PhysicsUtils
	PhysicsWorld
	Rasterizer
	RenderObjects
	RigidBody
	Shader
	Window
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()