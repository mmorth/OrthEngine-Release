add_library(RenderObjects STATIC 
	RenderObject.cpp RenderObject.hpp InstancedObject.cpp InstancedObject.hpp NonInstancedObject.cpp NonInstancedObject.hpp
	GeometricObjects/GeometricInstancedObject.cpp GeometricObjects/GeometricInstancedObject.hpp GeometricObjects/GeometricNonInstancedObject.cpp GeometricObjects/GeometricNonInstancedObject.hpp GeometricObjects/GeometricObjectStructs.hpp
	LightObjects/LightObject.cpp LightObjects/LightObject.hpp LightObjects/DirectionalLight.cpp LightObjects/DirectionalLight.hpp LightObjects/PointLight.cpp LightObjects/PointLight.hpp LightObjects/SpotLight.cpp LightObjects/SpotLight.hpp LightObjects/LightObjectStructs.hpp
	OtherRenders/SkyboxObject.cpp OtherRenders/SkyboxObject.hpp OtherRenders/OtherRenderStructs.hpp OtherRenders/TextObject.hpp OtherRenders/TextObject.cpp)
target_include_directories(RenderObjects
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data)
target_link_libraries(RenderObjects PRIVATE 
	Camera
	MathUtils
	ObjectUtils
	Rasterizer
	Shader
	Texture
	Transform
	Window
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
