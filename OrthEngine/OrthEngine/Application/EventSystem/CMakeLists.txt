# Add stb_image library
add_library(Publisher STATIC Publisher.cpp Publisher.hpp)

target_include_directories(Publisher PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(Publisher ${G3LOG_LIBRARIES})

if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
