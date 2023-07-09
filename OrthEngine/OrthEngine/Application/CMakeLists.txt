add_library(EngineApp STATIC EngineApp.cpp EngineApp.hpp)
target_include_directories(EngineApp 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data)
target_link_libraries(EngineApp PRIVATE 
	Camera
	Factories
	Managers
	Texture
	Transform
	Window
)

add_subdirectory(EventSystem)

add_subdirectory(tests)
