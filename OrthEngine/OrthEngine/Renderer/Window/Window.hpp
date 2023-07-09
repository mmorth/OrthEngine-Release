#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <iostream>
#include <map>
#include <string>
#include <utility>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Camera/Camera.hpp"
#include "MathUtils.hpp"
#include "Publisher.hpp"
#include "WindowStructs.hpp"

class Window 
{
public:
	Window(const WindowSettings& windowSettings);
	Window() : m_window(nullptr) {}
	~Window();

	// constructor functions
	virtual void createAndInitializeWindow(const WindowSettings& windowSettings);
	virtual void initializeGLFW(const WindowHints& windowHints);
	virtual void createWindow(const WindowLayout& windowLayout);

	// state getters/setters
	virtual CameraMovement processInput();
	virtual bool shouldCloseWindow();
	virtual void swapBuffers();
	virtual void pollEvents();
	virtual double getGlfwTime();
	virtual void setWindowTitle(const std::string& title);
	virtual void getWindowSize(int& width, int& height);
	static void setWindowBackgroundColor(const MathUtils::Vec4& color, const bool clearAll);

	// EventSystem publishers
	static void setCallbacks(std::shared_ptr<Publisher<std::pair<double, double>>> cursorPublisher, std::shared_ptr<Publisher<std::pair<double, double>>> scrollPublisher, std::shared_ptr<Publisher<std::pair<double, double>>> windowPublisher);
	static std::shared_ptr<Publisher<std::pair<double, double>>> s_cursorPosPublisher;
	static std::shared_ptr<Publisher<std::pair<double, double>>> s_scrollPosPublisher;
	static std::shared_ptr<Publisher<std::pair<double, double>>> s_windowSizePublisher;

protected:
	// test enabled functions
	virtual void setWindowSize(const int width, const int height);
	virtual GLFWwindow* getCurrentContext();
	virtual GLFWwindow* getCurrentWindow();
	virtual int getCursorMode();
	virtual void getCursorPosition(double& xpos, double& ypos);
	virtual void getViewportDimensions(int& width, int& height);

	// mockable functions
	virtual void cleanUp();
	virtual bool loadGladGLLoader();
	virtual void unlimitFps();
	virtual bool checkKeyPressed(const int key);

	// callbacks
	using CursorPosCallbackType = void(*)(GLFWwindow*, double, double);
	CursorPosCallbackType m_cursorPosCallback =
		[](GLFWwindow* window, double x, double y)  
	{
		s_cursorPosPublisher->publish(std::make_pair(x, y));
		int windowWidth, windowHeight;
		glfwGetWindowSize(window, &windowWidth, &windowHeight);
		glfwSetCursorPos(window, windowWidth/2, windowHeight/2);
	};

	using ScrollCallbackType = void(*)(GLFWwindow*, double, double);
	ScrollCallbackType m_scrollCallback = [](GLFWwindow* window, double xOffset, double yOffset) 
	{
		Window::s_scrollPosPublisher->publish(std::make_pair(xOffset, yOffset));
	};

	using FramebufferSizeCallbackType = void(*)(GLFWwindow*, int, int);
	FramebufferSizeCallbackType m_framebufferSizeCallback = [](GLFWwindow* window, int width, int height) 
	{
		glViewport(0, 0, width, height); // update default framebuffer size
		Window::s_windowSizePublisher->publish(std::make_pair(width, height)); // update secondary framebuffer sizes
	};

private:
	GLFWwindow* m_window;

	std::map<int, CameraMovement> m_keyActionMap;
};

#endif // WINDOW_HPP