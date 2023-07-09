# Add glad library
add_library(glad glad.c)

find_package(glfw3 REQUIRED)
target_link_libraries(glad glfw)

# Add subdirectores
add_subdirectory(stb_image)