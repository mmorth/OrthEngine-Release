#ifndef ENGINEAPP_HPP
#define ENGINEAPP_HPP

#include <chrono>
#include <iostream>
#include <vector>

#include "Camera/Camera.hpp"
#include "EventSystem/Publisher.hpp"
#include "Factories/CameraFactory.hpp"
#include "Managers/RenderObjectManager.hpp"
#include "Window/Window.hpp"

class EngineApp
{
public:
	EngineApp(const bool isTestRender = false);
	~EngineApp();

	virtual void EngineApp::initialize(const std::shared_ptr<Window> window, const std::shared_ptr<RenderObjectManager> renderObjectManager, const std::shared_ptr<Camera> camera);
	virtual void run();

protected:
	std::shared_ptr<Window> m_window;
	std::shared_ptr<RenderObjectManager> m_renderObjectManager;
	std::shared_ptr<Camera> m_camera;

	float m_deltaTime;
	float m_lastFrame;

	bool m_isTestRender;

	virtual void calculateFps(std::chrono::time_point<std::chrono::high_resolution_clock>& start, unsigned int& frames);
	virtual std::chrono::time_point<std::chrono::high_resolution_clock> getCurrentTime();
};

#endif // ENGINEAPP_HPP
