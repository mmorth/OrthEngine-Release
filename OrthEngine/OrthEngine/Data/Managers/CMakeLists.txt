add_library(Managers STATIC RenderObjectSceneCreator.cpp RenderObjectSceneCreator.hpp RenderObjectManager.cpp RenderObjectManager.hpp)
target_include_directories(Managers 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/Managers)
target_link_libraries(Managers PRIVATE 
	Camera
	Factories
	Framebuffer
	GlobalSettingsConstants
	Rasterizer
	RenderObjects
	Shader
	Window
)

add_subdirectory(tests)