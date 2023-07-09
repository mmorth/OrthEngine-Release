#include <array>

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "PublisherMock.hpp"
#include "Window/Window.hpp"


// ===============================================================
// WindowTestable
// ===============================================================
class WindowTestable : public Window {
public:
    WindowTestable(const WindowSettings& windowSettings)
        : Window(windowSettings) {}
    WindowTestable() : Window() {}

    // ===============================================================
    // Dependency Injection Functions
    // ===============================================================
    bool initializedGLFWCalled = false;
    void initializeGLFW(const WindowHints& windowHints) override
    {
        initializedGLFWCalled = true;
        if (!windowHints.testingError)
            Window::initializeGLFW(windowHints);
    }

    bool gladInitializeCalled = false;
    bool loadGladGLLoader() override
    {
        gladInitializeCalled = true;
        return Window::loadGladGLLoader();
    }

    // ===============================================================
    // Public GLFW Window Accessors
    // ===============================================================
    GLFWwindow* getWindowContext() 
    {
        return Window::getCurrentContext();
    }

    GLFWwindow* getActiveWindow()
    {
        return Window::getCurrentWindow();
    }

    int getWindowCursorMode()
    {
        return Window::getCursorMode();
    }

    void getWindowCursorPosition(double& xpos, double& ypos)
    {
        Window::getCursorPosition(xpos, ypos);
    }

    void getDefaultFramebufferDimensions(int& width, int& height)
    {
        Window::getViewportDimensions(width, height);
    }

    void setGLFWWindowSize(int width, int height)
    {
        Window::setWindowSize(width, height);
    }

    void getGLFWWindowSize(int& width, int& height)
    {
        Window::getWindowSize(width, height);
    }

    // ===============================================================
    // Callback Wrappers
    // ===============================================================
    void callCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
    {
        m_cursorPosCallback(window, xpos, ypos);
    }

    void callScrollPosPublisher(GLFWwindow* window, double xOffset, double yOffset)
    {
        m_scrollCallback(window, xOffset, yOffset);
    }

    void callFramebufferSizeCallback(GLFWwindow* window, int width, int height)
    {
        m_framebufferSizeCallback(window, width, height);
    }

    // ===============================================================
    // Mocks
    // ===============================================================
    MOCK_METHOD(void, cleanUp, ());
    MOCK_METHOD(void, unlimitFps, ());
    MOCK_METHOD(bool, checkKeyPressed, (int key), (override));

    // ===============================================================
    // Members
    // ===============================================================
    std::shared_ptr<PublisherMock<std::pair<double, double>>> s_cursorPosPublisherMock = std::make_shared<PublisherMock<std::pair<double, double>>>();
    std::shared_ptr<PublisherMock<std::pair<double, double>>> s_scrollPublisherMock = std::make_shared<PublisherMock<std::pair<double, double>>>();
    std::shared_ptr<PublisherMock<std::pair<double, double>>> s_windowPublisherMock = std::make_shared<PublisherMock<std::pair<double, double>>>();
};


// ===============================================================
// WindowTest
// ===============================================================
class WindowTest : public testing::Test {
public:
    static WindowSettings s_windowSettings;

protected:
    void SetUp() override {}
    void TearDown() override {}

    void expectKeyPressed(::testing::StrictMock<WindowTestable>& windowTestable, int keyPressed);
};

WindowSettings WindowTest::s_windowSettings = { {{1200, 800}, "Test Window", true}, {3, 3, 4, false} };

// ===============================================================
// Test Functions
// ===============================================================
// ------------------------------------------------------------------------
TEST_F(WindowTest, CreateWindowConfigsCorrectGLFWProperties)
{
    ::testing::StrictMock<WindowTestable> windowTestable;

    // FUT
    windowTestable.createAndInitializeWindow(WindowTest::s_windowSettings);

    // verify functions called during execution
    EXPECT_EQ(true, windowTestable.initializedGLFWCalled);
    EXPECT_EQ(true, windowTestable.gladInitializeCalled);

    // verify window state after constructor
    EXPECT_EQ(windowTestable.getActiveWindow(), windowTestable.getWindowContext());
    EXPECT_EQ(GLFW_CURSOR_HIDDEN, windowTestable.getWindowCursorMode());
    double xpos, ypos;
    windowTestable.getWindowCursorPosition(xpos, ypos);
    EXPECT_EQ(s_windowSettings.windowLayout.dimensions.x/2, xpos);
    EXPECT_EQ(s_windowSettings.windowLayout.dimensions.y/2, ypos);

    // verify publisher's are non-null
    EXPECT_TRUE(Window::s_cursorPosPublisher != nullptr);
    EXPECT_TRUE(Window::s_scrollPosPublisher != nullptr);
    EXPECT_TRUE(Window::s_windowSizePublisher != nullptr);

    // verify GLFW time is non-zero
    EXPECT_TRUE(windowTestable.getGlfwTime() > 0);
}

// ------------------------------------------------------------------------
TEST_F(WindowTest, CursorScrollAndFramebufferSizeSetAndPublishCorrectInformation)
{
    // construct Window and set PublisherMock callbacks
    ::testing::StrictMock<WindowTestable> windowTestable(WindowTest::s_windowSettings);
    WindowTestable::setCallbacks(windowTestable.s_cursorPosPublisherMock, windowTestable.s_scrollPublisherMock, windowTestable.s_windowPublisherMock);

    // Callback parameter inputs
    int expectedX = 100;
    int expectedY = 150;

    // FUT Cursor callback
    EXPECT_CALL(*windowTestable.s_cursorPosPublisherMock, publish(::testing::Pair(::testing::Eq(expectedX), ::testing::DoubleEq(expectedY)))).Times(1);
    windowTestable.callCursorPosCallback(windowTestable.getActiveWindow(), expectedX, expectedY);
    // cursor should be moved back to center of screen
    double xpos, ypos;
    windowTestable.getWindowCursorPosition(xpos, ypos);
    EXPECT_EQ(s_windowSettings.windowLayout.dimensions.x/2, xpos);
    EXPECT_EQ(s_windowSettings.windowLayout.dimensions.y/2, ypos);

    // FUT Scroll callback
    EXPECT_CALL(*windowTestable.s_scrollPublisherMock, publish(::testing::Pair(::testing::Eq(expectedX), ::testing::DoubleEq(expectedY)))).Times(1);
    windowTestable.callScrollPosPublisher(windowTestable.getActiveWindow(), expectedX, expectedY);

    // FUT Framebuffer callback
    EXPECT_CALL(*windowTestable.s_windowPublisherMock, publish(::testing::Pair(::testing::Eq(expectedX), ::testing::DoubleEq(expectedY)))).Times(1);
    windowTestable.callFramebufferSizeCallback(windowTestable.getActiveWindow(), expectedX, expectedY);
    int width, height;
    windowTestable.getDefaultFramebufferDimensions(width, height);
    EXPECT_EQ(expectedX, width);
    EXPECT_EQ(expectedY, height);
}

// ------------------------------------------------------------------------
TEST_F(WindowTest, CreatingInvalidWindowThrowsException) 
{
    // create invalid window settings                                                       prevent GLFW from initializing
    WindowSettings invalidWindowSettings = { {{1200, 800}, "Test Window", false}, {3, 3, 4, true} };
    ::testing::StrictMock<WindowTestable> windowTestable;

    // raii cleanup before throwing exception
    EXPECT_CALL(windowTestable, cleanUp).Times(1);

    // FUT
    EXPECT_THROW(windowTestable.createAndInitializeWindow(invalidWindowSettings), std::runtime_error);
}

// ------------------------------------------------------------------------
TEST_F(WindowTest, WindowCreationWithUnlimitFpsSetsGLFWUpdateInvervalTo0) 
{
    // create invalid window settings
    WindowSettings invalidWindowSettings = { {{1200, 800}, "Test Window", false}, {3, 3, 4, false} };
    ::testing::StrictMock<WindowTestable> windowTestable;

    EXPECT_CALL(windowTestable, unlimitFps).Times(1);

    // FUT
    windowTestable.createAndInitializeWindow(invalidWindowSettings);
}

// ------------------------------------------------------------------------
void WindowTest::expectKeyPressed(::testing::StrictMock<WindowTestable>& windowTestable, int keyPressed)
{
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_ESCAPE)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_ESCAPE));
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_W)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_W));
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_S)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_S));
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_A)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_A));
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_D)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_D));
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_Q)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_Q));
    EXPECT_CALL(windowTestable, checkKeyPressed(GLFW_KEY_E)).WillOnce(::testing::Return(keyPressed == GLFW_KEY_E));
}

// ------------------------------------------------------------------------
TEST_F(WindowTest, WindowProcessInputCorrectlyReturnsActionBasedOnKeyPress) 
{
    ::testing::StrictMock<WindowTestable> windowTestable(WindowTest::s_windowSettings);

    // check expected key presses
    expectKeyPressed(windowTestable, GLFW_KEY_W); EXPECT_EQ(windowTestable.processInput(), CameraMovement::FORWARD); // FUT
    expectKeyPressed(windowTestable, GLFW_KEY_S); EXPECT_EQ(windowTestable.processInput(), CameraMovement::BACKWARD); // FUT
    expectKeyPressed(windowTestable, GLFW_KEY_A); EXPECT_EQ(windowTestable.processInput(), CameraMovement::LEFT); // FUT
    expectKeyPressed(windowTestable, GLFW_KEY_D); EXPECT_EQ(windowTestable.processInput(), CameraMovement::RIGHT); // FUT
    expectKeyPressed(windowTestable, GLFW_KEY_Q); EXPECT_EQ(windowTestable.processInput(), CameraMovement::ROLL_LEFT); // FUT
    expectKeyPressed(windowTestable, GLFW_KEY_E); EXPECT_EQ(windowTestable.processInput(), CameraMovement::ROLL_RIGHT); // FUT

    // check unexpected key press doesn't do anything
    expectKeyPressed(windowTestable, GLFW_KEY_M); EXPECT_EQ(windowTestable.processInput(), CameraMovement::NONE); // FUT

    // check escape key correctly signals to end window loop
    ASSERT_EQ(false, windowTestable.shouldCloseWindow());
    expectKeyPressed(windowTestable, GLFW_KEY_ESCAPE); EXPECT_EQ(windowTestable.processInput(), CameraMovement::NONE); // FUT
    ASSERT_EQ(true, windowTestable.shouldCloseWindow());
}

// ------------------------------------------------------------------------
TEST_F(WindowTest, WindowResizingProperlyResizesTheGLFWWindow) 
{
    ::testing::StrictMock<WindowTestable> windowTestable(WindowTest::s_windowSettings);

    int newWidth = 200;
    int newHeight = 150;
    windowTestable.setGLFWWindowSize(newWidth, newHeight); // FUT
    int width, height;
    windowTestable.getGLFWWindowSize(width, height); // FUT
    EXPECT_EQ(newWidth, width);
    EXPECT_EQ(newHeight, height);
}

// ------------------------------------------------------------------------
TEST_F(WindowTest, SetWindowColorCorrectlyClearsAllBuffersAndSetsColorBufferToSpecifiedColor)
{
    ::testing::StrictMock<WindowTestable> windowTestable(WindowTest::s_windowSettings);

    MathUtils::Vec4 color{ 1.0f, 0.0f, 0.0f, 1.0f };
    windowTestable.setWindowBackgroundColor(color, true); // FUT

    // Verify color buffer clearing
    std::array<unsigned char, 4> pixelColor;
    glReadPixels(0, 0, 1, 1, GL_RGBA, GL_UNSIGNED_BYTE, pixelColor.data());
    std::array<unsigned char, 4> expectedColor{0xFF, 0x00, 0x00, 0xFF}; // red color
    EXPECT_EQ(pixelColor, expectedColor);

    // Verify depth buffer clearing
    GLfloat depthValue;
    glReadPixels(0, 0, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &depthValue);
    EXPECT_EQ(depthValue, 1.0f);  // Expected depth value of 1.0 representing farthest depth

    // Verify stencil buffer clearing
    GLint stencilValue;
    glReadPixels(0, 0, 1, 1, GL_STENCIL_INDEX, GL_INT, &stencilValue);
    EXPECT_EQ(stencilValue, 0x00); // default empty stencil buffer
}
