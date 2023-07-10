#include "Window.hpp"

// ------------------------------------------------------------------------
std::shared_ptr<Publisher<std::pair<double, double>>> Window::s_cursorPosPublisher;
std::shared_ptr<Publisher<std::pair<double, double>>> Window::s_scrollPosPublisher;
std::shared_ptr<Publisher<std::pair<double, double>>> Window::s_windowSizePublisher;


// ===============================================================
// Constructors and Destructors
// ===============================================================

// ------------------------------------------------------------------------
Window::Window(const WindowSettings& windowSettings)
{
    LOG(INFO) << "ctor";
    
    createAndInitializeWindow(windowSettings);
}

// ------------------------------------------------------------------------
Window::~Window()
{
    cleanUp();

    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void Window::createAndInitializeWindow(const WindowSettings& windowSettings)
{
    // Initlize all possible key actions
    m_keyActionMap = { {GLFW_KEY_W, CameraMovement::FORWARD}, {GLFW_KEY_S, CameraMovement::BACKWARD},
                        {GLFW_KEY_A, CameraMovement::LEFT}, {GLFW_KEY_D, CameraMovement::RIGHT},
                        {GLFW_KEY_Q, CameraMovement::ROLL_LEFT}, {GLFW_KEY_E, CameraMovement::ROLL_RIGHT} };

    // Set all Publisher callbacks to default value
    setCallbacks(std::make_shared<Publisher<std::pair<double, double>>>(), std::make_shared<Publisher<std::pair<double, double>>>(), std::make_shared<Publisher<std::pair<double, double>>>());

    initializeGLFW(windowSettings.windowHints);

    // Create GLFW window based on size and name paramters
    createWindow(windowSettings.windowLayout);

    // Load GLAD for OpenGL function pointers
    if (!loadGladGLLoader())
        throw std::runtime_error("Failed to load glad function pointers");
}

// ------------------------------------------------------------------------
void Window::initializeGLFW(const WindowHints& windowHints)
{
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, windowHints.majorVersion);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, windowHints.minorVersion);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, windowHints.numSamples);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
}

// ------------------------------------------------------------------------
void Window::createWindow(const WindowLayout& windowLayout)
{
    // Create window and set to current context
    m_window = glfwCreateWindow(static_cast<int>(windowLayout.dimensions.x), static_cast<int>(windowLayout.dimensions.y), windowLayout.name.c_str(), nullptr, nullptr);
    if (m_window == NULL)
    {
        cleanUp();
        throw std::runtime_error("Failed to create GLFW window");
    }
    glfwMakeContextCurrent(m_window);

    // Hide cursor and set position to middle of screen
    glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    int windowWidth, windowHeight;
    getWindowSize(windowWidth, windowHeight);
    glfwSetCursorPos(m_window, windowWidth / 2, windowHeight / 2);

    // Setup callbacks for cursor and scroll events
    glfwSetCursorPosCallback(m_window, m_cursorPosCallback);
    glfwSetScrollCallback(m_window, m_scrollCallback);
    glfwSetFramebufferSizeCallback(m_window, m_framebufferSizeCallback);

    // Disable V-Sync to enable unlimited FPS (instead of default 60 fps cap)
    if (!windowLayout.limitFPS)
        unlimitFps();

    LOG(INFO) << "Window Created";
}


// ===============================================================
// Window State Getters and Setters
// ===============================================================

// ------------------------------------------------------------------------
CameraMovement Window::processInput()
{
    // Determine whether any keyboard input received
    if (checkKeyPressed(GLFW_KEY_ESCAPE))
        glfwSetWindowShouldClose(m_window, true);

    // Determine the movement action based on whether any key is pressed
    CameraMovement movementResult = CameraMovement::NONE;
    for (const auto& pair : m_keyActionMap) 
    {
        if (checkKeyPressed(pair.first))
            movementResult = pair.second;
    }

    return movementResult;
}

// ------------------------------------------------------------------------
bool Window::shouldCloseWindow()
{
    return glfwWindowShouldClose(m_window);
}

// ------------------------------------------------------------------------
void Window::swapBuffers()
{
    glfwSwapBuffers(m_window);
}

// ------------------------------------------------------------------------
void Window::pollEvents()
{
    glfwPollEvents();
}

// ------------------------------------------------------------------------
void Window::setWindowTitle(const std::string& title)
{
    glfwSetWindowTitle(m_window, title.c_str());
}

// ------------------------------------------------------------------------
double Window::getGlfwTime()
{
    return glfwGetTime();
}

// ------------------------------------------------------------------------
void Window::getWindowSize(int& width, int& height)
{
    glfwGetWindowSize(m_window, &width, &height);
}

// ------------------------------------------------------------------------
void Window::setWindowBackgroundColor(const MathUtils::Vec4& color, const bool clearAll)
{
    glClearColor(color.x, color.y, color.z, color.w);
    if (clearAll)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    else
        glClear(GL_COLOR_BUFFER_BIT);
}


// ===============================================================
// Test Accessibility Functions
// ===============================================================

// ------------------------------------------------------------------------
void Window::setCallbacks(std::shared_ptr<Publisher<std::pair<double, double>>> cursorPublisher, std::shared_ptr<Publisher<std::pair<double, double>>> scrollPublisher, std::shared_ptr<Publisher<std::pair<double, double>>> windowPublisher)
{
    Window::s_cursorPosPublisher = cursorPublisher;
    Window::s_scrollPosPublisher = scrollPublisher;
    Window::s_windowSizePublisher = windowPublisher;
}

// ------------------------------------------------------------------------
void Window::setWindowSize(const int width, const int height)
{
    glfwSetWindowSize(m_window, width, height);
}

// ------------------------------------------------------------------------
GLFWwindow* Window::getCurrentContext()
{
    return glfwGetCurrentContext();
}

// ------------------------------------------------------------------------
GLFWwindow* Window::getCurrentWindow()
{
    return m_window;
}

// ------------------------------------------------------------------------
int Window::getCursorMode()
{
    return glfwGetInputMode(m_window, GLFW_CURSOR);
}

// ------------------------------------------------------------------------
void Window::getCursorPosition(double& xpos, double& ypos)
{
    glfwGetCursorPos(m_window, &xpos, &ypos);
}

// ------------------------------------------------------------------------
void Window::getViewportDimensions(int& width, int& height)
{
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);

    width = viewport[2];
    height = viewport[3];
}

// ------------------------------------------------------------------------
void Window::cleanUp()
{
    glfwTerminate();
}

// ------------------------------------------------------------------------
bool Window::loadGladGLLoader()
{
    return gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
}

// ------------------------------------------------------------------------
void Window::unlimitFps()
{
    glfwSwapInterval(0);
    LOG(INFO) << "Unlimited FPS";
}

// ------------------------------------------------------------------------
bool Window::checkKeyPressed(const int key)
{
    return glfwGetKey(m_window, key) == GLFW_PRESS;
}
