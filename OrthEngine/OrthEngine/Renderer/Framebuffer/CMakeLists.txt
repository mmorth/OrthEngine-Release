# Find the GLFW package
find_package(GLFW REQUIRED)

# Create the Window library
add_library(Framebuffer STATIC Framebuffer.cpp Framebuffer.hpp)
target_include_directories(Framebuffer 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data)
target_link_libraries(Framebuffer PUBLIC 
	MathUtils
	GLFW
	GlobalSettingsConstants
	glad
	Rasterizer
	Shader
)

add_subdirectory(tests)
