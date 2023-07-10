add_library(GlobalSettingsConstants STATIC GlobalSettings.cpp GlobalSettings.hpp GlobalConstants.hpp)
target_include_directories(GlobalSettingsConstants 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/GlobalSettingsConstants
)

target_link_libraries(GlobalSettingsConstants ${G3LOG_LIBRARIES})

configure_file(${CMAKE_SOURCE_DIR}/OrthEngine/Data/GlobalSettingsConstants/GlobalConfig.json ${CMAKE_BINARY_DIR}/bin/GlobalConfig.json COPYONLY)

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
