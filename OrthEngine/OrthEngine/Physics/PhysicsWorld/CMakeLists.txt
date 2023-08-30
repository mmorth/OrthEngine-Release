# Find the GLFW package
find_package(Bullet REQUIRED)

# Create the Window library
add_library(PhysicsWorld STATIC PhysicsWorld.cpp PhysicsWorld.hpp PhysicsWorldImpl.hpp PhysicsWorldImpl.cpp)
target_include_directories(PhysicsWorld 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE
		${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math)
target_link_libraries(PhysicsWorld PUBLIC 
	MathUtils
	ObjectUtils
	RigidBody
	${BULLET_LIBRARIES}
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
