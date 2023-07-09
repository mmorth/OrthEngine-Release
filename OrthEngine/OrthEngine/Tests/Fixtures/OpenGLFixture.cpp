#include "OpenGLFixture.hpp"

// Define the static member variable
GLFWwindow* OpenGLTestFixture::window = nullptr;

void OpenGLTestFixture::SetUpTestCase()
{
    // Initialize GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    window = glfwCreateWindow(800, 600, "OpenGL Test", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    // Make the OpenGL context current
    glfwMakeContextCurrent(window);

    // Load Glad
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        throw std::runtime_error("Failed to initialize Glad");
    }
}

void OpenGLTestFixture::TearDownTestCase()
{
    // Terminate GLFW and destroy the window
    glfwTerminate();
    glfwDestroyWindow(window);
}