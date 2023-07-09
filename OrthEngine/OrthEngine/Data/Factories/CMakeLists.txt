add_library(Factories STATIC CameraFactory.cpp CameraFactory.hpp FactoryStructs.hpp RasterizerFactory.cpp RasterizerFactory.hpp RenderObjectFactory.cpp RenderObjectFactory.hpp ShaderFactory.cpp ShaderFactory.hpp VertexDataFactory.cpp VertexDataFactory.hpp FramebufferFactory.hpp FramebufferFactory.cpp)
target_include_directories(Factories 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories)
target_link_libraries(Factories PRIVATE 
	Camera
	Framebuffer
	GlobalSettingsConstants
	Rasterizer
	RenderObjects
	Shader
	Window
)

add_subdirectory(tests)
