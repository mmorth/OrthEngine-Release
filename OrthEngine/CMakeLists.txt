# Build options
option(BUILD_TESTS "Build Unit and Integration Tests" OFF)

# Root-level project CMake
cmake_minimum_required (VERSION 3.17)

project ("OrthEngine")

# Include sub-projects.
add_subdirectory ("OrthEngine")
