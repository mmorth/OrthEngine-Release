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
	${G3LOG_LIBRARIES}
)

add_subdirectory(EventSystem)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
