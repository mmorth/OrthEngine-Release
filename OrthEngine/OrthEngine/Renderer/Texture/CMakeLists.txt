# Find the GLFW package
find_package(GLFW REQUIRED)

# Create the Window library
add_library(Texture STATIC TextureFactory.cpp TextureFactory.hpp)
target_include_directories(Texture PUBLIC 
    "${CMAKE_SOURCE_DIR}/OrthEngine/Externals"
)
target_link_libraries(Texture PUBLIC 
	MathUtils
	GLFW
	glad
	stb_image
)

add_subdirectory(tests)
