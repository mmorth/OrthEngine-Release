find_package(Freetype REQUIRED)

# Create the Window library
add_library(Rasterizer STATIC Rasterizer.cpp Rasterizer.hpp InstancedRasterizer.cpp InstancedRasterizer.hpp NonInstancedRasterizer.cpp NonInstancedRasterizer.hpp SkyboxRasterizer.cpp SkyboxRasterizer.hpp TextRasterizer.cpp TextRasterizer.hpp RasterizerUtility.cpp RasterizerUtility.hpp DataStructs.hpp)
target_link_libraries(Rasterizer PUBLIC 
	ASSIMP
	MathUtils
	GlobalSettingsConstants
	GLFW
	GLM
	glad
	${FREETYPE_LIBRARIES}
	${G3LOG_LIBRARIES}
)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
