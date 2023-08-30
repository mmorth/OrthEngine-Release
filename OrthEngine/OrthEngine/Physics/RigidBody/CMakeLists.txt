# Find the GLFW package
find_package(Bullet REQUIRED)

# Create the Window library
add_library(RigidBody STATIC RigidBody.cpp RigidBody.hpp RigidBodyImpl.hpp RigidBodyImpl.cpp)
target_include_directories(RigidBody 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE
		${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math
		${CMAKE_SOURCE_DIR}/OrthEngine)
target_link_libraries(RigidBody PUBLIC 
	MathUtils
	ObjectUtils
	PhysicsUtils
	${BULLET_LIBRARIES}
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
