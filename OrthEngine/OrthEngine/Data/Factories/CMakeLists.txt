add_library(Factories STATIC CameraFactory.cpp CameraFactory.hpp FactoryStructs.hpp RasterizerFactory.cpp RasterizerFactory.hpp RenderObjectFactory.cpp RenderObjectFactory.hpp ShaderFactory.cpp ShaderFactory.hpp VertexDataFactory.cpp VertexDataFactory.hpp FramebufferFactory.hpp FramebufferFactory.cpp "RigidBodyFactory.hpp" "RigidBodyFactory.cpp")
target_include_directories(Factories 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/Factories
		${CMAKE_SOURCE_DIR}/OrthEngine/Utils/Object)
target_link_libraries(Factories PRIVATE 
	Camera
	Framebuffer
	GlobalSettingsConstants
	ObjectUtils
	Rasterizer
	RenderObjects
	RigidBody
	PhysicsUtils
	Shader
	Window
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
