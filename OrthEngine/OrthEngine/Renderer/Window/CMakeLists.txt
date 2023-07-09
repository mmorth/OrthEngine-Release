# Find the GLFW package
find_package(GLFW REQUIRED)

# Create the Window library
add_library(Window STATIC Window.cpp Window.hpp WindowStructs.hpp)

target_include_directories(Window 
	PRIVATE ${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Math 
	${CMAKE_SOURCE_DIR}/OrthEngine/Application/EventSystem/tests
)

target_link_libraries(Window PUBLIC 
	Camera	
	GLFW
	glad
	MathUtils
	Publisher
)

add_subdirectory(tests)
