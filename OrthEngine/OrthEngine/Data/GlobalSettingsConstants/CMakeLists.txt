add_library(GlobalSettingsConstants STATIC GlobalSettings.cpp GlobalSettings.hpp GlobalConstants.hpp)
target_include_directories(GlobalSettingsConstants 
	PUBLIC 
		${CMAKE_SOURCE_DIR}/OrthEngine/Data/GlobalSettingsConstants)

configure_file(${CMAKE_SOURCE_DIR}/OrthEngine/Data/GlobalSettingsConstants/GlobalConfig.json ${CMAKE_BINARY_DIR}/bin/GlobalConfig.json COPYONLY)

add_subdirectory(tests)
