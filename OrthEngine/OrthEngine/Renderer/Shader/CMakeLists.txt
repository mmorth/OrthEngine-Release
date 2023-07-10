# Find the GLFW package
find_package(GLFW REQUIRED)

# Create the Window library
add_library(Shader STATIC Shader.cpp Shader.hpp)
target_include_directories(Shader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/shaders)
target_link_libraries(Shader PUBLIC 
	MathUtils
	GLFW
	GLM
	glad
	${G3LOG_LIBRARIES}
)

# Add shader files to CMake output
# Get a list of all files in the resources directory
file(GLOB SHADER_SOURCES "${CMAKE_SOURCE_DIR}/OrthEngine/Renderer/Shader/shaders/*")

# Loop through the list of files and copy each file to the binary directory
foreach(file ${SHADER_SOURCES})
  get_filename_component(filename ${file} NAME)
  configure_file(${file} ${CMAKE_BINARY_DIR}/bin COPYONLY)
endforeach()

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
