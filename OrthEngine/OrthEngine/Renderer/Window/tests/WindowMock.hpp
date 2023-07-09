#ifndef WINDOWMOCK_HPP
#define WINDOWMOCK_HPP

#include <gmock/gmock.h>

#include "Window/Window.hpp"

class WindowMock : public Window 
{
public:
    MOCK_METHOD(void, createAndInitializeWindow, (const WindowSettings& windowSettings), (override));
    MOCK_METHOD(void, initializeGLFW, (const WindowHints& windowHints), (override));
    MOCK_METHOD(void, createWindow, (const WindowLayout& windowLayout), (override));
    MOCK_METHOD(CameraMovement, processInput, (), (override));
    MOCK_METHOD(bool, shouldCloseWindow, (), (override));
    MOCK_METHOD(void, swapBuffers, (), (override));
    MOCK_METHOD(void, pollEvents, (), (override));
    MOCK_METHOD(double, getGlfwTime, (), (override));
    MOCK_METHOD(void, setWindowTitle, (const std::string& title), (override));
    MOCK_METHOD(void, getWindowSize, (int& width, int& height), (override));
    MOCK_METHOD(void, setWindowSize, (const int width, const int height), (override));
    MOCK_METHOD(GLFWwindow*, getCurrentContext, (), (override));
    MOCK_METHOD(GLFWwindow*, getCurrentWindow, (), (override));
    MOCK_METHOD(int, getCursorMode, (), (override));
    MOCK_METHOD(void, getCursorPosition, (double& xpos, double& ypos), (override));
    MOCK_METHOD(void, getViewportDimensions, (int& width, int& height), (override));
    MOCK_METHOD(void, cleanUp, (), (override));
    MOCK_METHOD(bool, loadGladGLLoader, (), (override));
    MOCK_METHOD(void, unlimitFps, (), (override));
    MOCK_METHOD(bool, checkKeyPressed, (const int key), (override));
};

#endif // WINDOWMOCK_HPP
