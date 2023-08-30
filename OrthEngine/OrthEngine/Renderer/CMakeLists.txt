# Find GLFW
find_package(glfw3 REQUIRED)

# Create library and link targets
add_library(GLFW INTERFACE)
target_include_directories(GLFW INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(GLFW INTERFACE glfw glad)

# Export GLFW as interface library to link to sub directories
export(TARGETS GLFW glad
	FILE ${CMAKE_PREFIX_PATH}/GLFWConfig.cmake
)


# Create GLM library and link targets
add_library(GLM INTERFACE)
target_include_directories(GLM INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Export GLFW as interface library to link to sub directories
export(TARGETS GLM glad
	FILE ${CMAKE_PREFIX_PATH}/GLMConfig.cmake
)

# Add subdirectories
add_subdirectory(Camera)
add_subdirectory(Framebuffer)
add_subdirectory(Rasterizers)
add_subdirectory(Shader)
add_subdirectory(Texture)
add_subdirectory(Transform)
add_subdirectory(Window)
