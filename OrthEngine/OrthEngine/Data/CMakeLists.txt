# Create ASSIMP library and link targets
find_package(assimp REQUIRED)
add_library(ASSIMP INTERFACE)
target_include_directories(ASSIMP INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(ASSIMP INTERFACE assimp::assimp)

# Export ASSIMP as interface library to link to sub directories
export(TARGETS ASSIMP
	FILE ${CMAKE_PREFIX_PATH}/ASSIMPConfig.cmake
)

add_subdirectory(Factories)
add_subdirectory(Managers)
add_subdirectory(GlobalSettingsConstants)
add_subdirectory(RenderObjects)

#add_subdirectory(tests)
