# Find the GLFW package
find_package(GLFW REQUIRED)
find_package(GLM REQUIRED)

# Create the Window library
add_library(Camera STATIC Camera.cpp Camera.hpp FirstPersonCamera.cpp FirstPersonCamera.hpp ThirdPersonCamera.cpp ThirdPersonCamera.hpp)
target_include_directories(Camera 
	PUBLIC 
		${CMAKE_CURRENT_SOURCE_DIR}/Camera
	PRIVATE
		${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math)
target_link_libraries(Camera PUBLIC 
	MathUtils
	GLFW
	glad
	GLM
)

add_subdirectory(tests)
