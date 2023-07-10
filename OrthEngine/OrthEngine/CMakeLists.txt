# Define project and prefix and include paths
set(PROJECT OrthEngine)
project(${PROJECT} VERSION 0.1.0)
cmake_minimum_required (VERSION 3.17)
set(CMAKE_SOURCE_DIR "C:\\Externals\\bin")
set(CMAKE_PREFIX_PATH "C:\\Externals\\lib")
set(CMAKE_INCLUDE_PATH "C:\\Externals\\include")
set(glfw3_DIR "C:\\Externals\\lib\\cmake\\glfw3")
set(assimp_DIR "C:\\Externals\\lib\\cmake\\assimp")
set(FREETYPE_INCLUDE_DIRS "C:\\Externals\\include\\freetype")
set(g3log_DIR "C:\\Externals\\lib\\cmake\\g3log")

# Set C++ version to 17
set(CMAKE_CXX_STANDARD 17)

# Add source to this project's executable.
add_executable (${PROJECT} "OrthEngine.cpp" "OrthEngine.hpp")

# Set the output directory for your executable
set_target_properties(${PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

# Define a custom command to copy the assimp and g3log dlls to the output directory
add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/assimp-vc142-mtd.dll"
        "$<TARGET_FILE_DIR:${PROJECT}>"
)
add_custom_command(TARGET ${PROJECT} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${CMAKE_SOURCE_DIR}/g3log.dll"
        "$<TARGET_FILE_DIR:${PROJECT}>"
)

# Add a custom target to make sure the dll is copied every time the project is built
add_custom_target(CopyDLL ALL DEPENDS ${PROJECT})

# Eigen
include_directories("C:\\Externals\\include\\eigen")

# ===== Enable Testing + Fetch GTest =====
include(CTest)
enable_testing()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ===== g3log Config =====
find_package(g3log REQUIRED)

# Add the include directories for g3log
include_directories(${G3LOG_INCLUDE_DIRS})

# Link against all required libraries
target_link_libraries(${PROJECT} PRIVATE
	Camera
	EngineApp
	MathUtils
	Window
    ${G3LOG_LIBRARIES}
)

# Add subdirectories
add_subdirectory(Application)
add_subdirectory(Externals)
add_subdirectory(Data)
add_subdirectory(Renderer)
add_subdirectory(Resources)
add_subdirectory(Utils)

if(BUILD_TESTS)
    add_subdirectory(Tests)
endif()
