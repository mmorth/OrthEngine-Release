# Create a library target for OpenGLFixture
add_library(OpenGLFixture STATIC OpenGLFixture.hpp OpenGLFixture.cpp)

target_include_directories(OpenGLFixture PUBLIC 
	"${CMAKE_SOURCE_DIR}/OrthEngine/Tests/Fixtures"
)

target_link_libraries(OpenGLFixture 
	GTest::gtest 
	GTest::gtest_main 
	GTest::gmock 
	GTest::gmock_main
)