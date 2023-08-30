# Find the GLFW package
find_package(Bullet REQUIRED)

# Create the Window library
add_library(PhysicsUtils STATIC BulletTypeConversions.cpp BulletTypeConversions.hpp)
target_include_directories(PhysicsUtils 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}
	PRIVATE
		${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math)
target_link_libraries(PhysicsUtils PUBLIC 
	MathUtils
	${BULLET_LIBRARIES}
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()