# Add stb_image library
add_library(Publisher STATIC Publisher.cpp Publisher.hpp)

target_include_directories(Publisher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(tests)
