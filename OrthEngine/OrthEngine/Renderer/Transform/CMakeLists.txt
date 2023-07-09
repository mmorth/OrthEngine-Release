# Find the GLFW package
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)

# Create the Window library
add_library(Transform STATIC Transform.cpp Transform.hpp)
target_include_directories(Transform 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Transforms
	PRIVATE
		${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math)
target_link_libraries(Transform PUBLIC 
	MathUtils
	GLFW
	glad
	GLM
	${G3LOG_LIBRARIES}
)

add_subdirectory(tests)