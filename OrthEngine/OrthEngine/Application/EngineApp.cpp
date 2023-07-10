#include "EngineApp.hpp"

// ------------------------------------------------------------------------
EngineApp::EngineApp(const bool isTestRender)
	: m_window(
		std::make_shared<Window>(
			WindowSettings(
				{ 
					{
						GlobalSettings::getInstance().getValue<float>("windowWidth", 1200), 
						GlobalSettings::getInstance().getValue<float>("windowHeight", 800) }, 
						GlobalSettings::getInstance().getValue<std::string>("windowName", "OrthEngine"), 
						GlobalSettings::getInstance().getValue<bool>("limitFps", true)
				},
				{
					GlobalSettings::getInstance().getValue<int>("majorVersion", 3), 
					GlobalSettings::getInstance().getValue<int>("minorVersion", 3), 
					GlobalSettings::getInstance().getValue<int>("numSamples", 4), 
					GlobalSettings::getInstance().getValue<bool>("testingError", false)
				}
			)
		)
	)
	, m_renderObjectManager(std::make_shared<RenderObjectManager>())
	, m_camera(CameraFactory::getInstance().getCamera(CameraTypes::FIRST_PERSON, MathUtils::Vec3{ 0.0f, 1.0f, 3.0f }))
	, m_deltaTime(0.0f)
	, m_lastFrame(0.0f)
	, m_isTestRender(isTestRender)
{
	LOG(INFO) << "ctor";

	// Initialize RenderObjectManager
	m_renderObjectManager->initialize();

	// Subscribe to Window events from Window
	m_window->s_cursorPosPublisher->subscribe(this, [this](std::pair<double, double> pos)
	{
		// Compute x and y mouse movements
		float xPos = static_cast<float>(pos.first);
		float yPos = static_cast<float>(pos.second);

		float xOffset = xPos - GlobalSettings::getInstance().getValue<float>("windowWidth", 1200)/2;
		float yOffset = GlobalSettings::getInstance().getValue<float>("windowHeight", 800)/2 - yPos;

		m_camera->ProcessMouseMovement(xOffset, yOffset);
	});

	// Subscribe to scroll events from Window
	m_window->s_scrollPosPublisher->subscribe(this, [this](std::pair<double, double> pos)
	{
		m_camera->ProcessMouseScroll(static_cast<float>(pos.second));
	});

	// Subscribe to Framebuffer window resizes
	m_window->s_windowSizePublisher->subscribe(this, [this](std::pair<int, int> screenSize)
	{
		m_renderObjectManager->updateFramebufferSize(screenSize.first, screenSize.second);
	});

	LOG(INFO) << "Window Callbacks set";
}

// ------------------------------------------------------------------------
EngineApp::~EngineApp() {}

// ------------------------------------------------------------------------
void EngineApp::initialize(const std::shared_ptr<Window> window, const std::shared_ptr<RenderObjectManager> renderObjectManager, const std::shared_ptr<Camera> camera)
{
	m_window = window;
	m_renderObjectManager = renderObjectManager;
	m_camera = camera;

	LOG(INFO) << "EngineApp mocks set";
}

// ------------------------------------------------------------------------
void EngineApp::run()
{
	// Variables for FPS computing
	std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::high_resolution_clock::now();
	unsigned int frames = 0;

	LOG(INFO) << "===== Main Render Loop =====";

	// Main render loop
	while (!m_window->shouldCloseWindow())
	{
		// Compute the time between frames for consistent behavior across different machines
		float currentFrame = static_cast<float>(m_window->getGlfwTime());
		m_deltaTime = currentFrame - m_lastFrame;
		m_lastFrame = currentFrame;

		// Process user input to see when window should close and set background color
		CameraMovement movementInput = m_window->processInput();
		m_camera->ProcessKeyboard(movementInput, m_deltaTime);
		Window::setWindowBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f }, true);

		// Get the window size
		int windowWidth, windowHeight;
		m_window->getWindowSize(windowWidth, windowHeight);

		RenderLoopConfigOptions renderLoopConfigOptions = { {m_camera->GetPosition(), m_camera->GetFront()}, {}, 
			{GlobalSettings::getInstance().getValue<bool>("framebufferEnabled", false), GlobalSettings::getInstance().getValue<bool>("msaaEnabled", false), GlobalSettings::getInstance().getValue<bool>("drawNormals", false), m_isTestRender} };
		m_renderObjectManager->renderAll(m_renderObjectManager->getTransformationMatrices(m_camera, windowWidth, windowHeight), renderLoopConfigOptions);

		// Swap buffers to prevent flickering or tearing in rasterization and poll user input events
		m_window->swapBuffers();
		m_window->pollEvents();

		// Calculate FPS
		calculateFps(start, frames);

		// Break out of the render loop if this is a test render
		if (m_isTestRender)
			break;
	}
}

// ------------------------------------------------------------------------
void EngineApp::calculateFps(std::chrono::time_point<std::chrono::high_resolution_clock>& start, unsigned int& frames)
{
	++frames;
	auto end = getCurrentTime();
	auto elapsed_seconds = std::chrono::duration_cast<std::chrono::duration<double>>(end - start);
	if (elapsed_seconds.count() >= 1.0) 
	{
		std::string fpsString = "FPS: " + std::to_string(frames / elapsed_seconds.count());
		m_renderObjectManager->updateFpsText(fpsString);
		m_window->setWindowTitle(fpsString);
		start = end;
		frames = 0;
	}
}

// ------------------------------------------------------------------------
std::chrono::time_point<std::chrono::high_resolution_clock> EngineApp::getCurrentTime()
{
	return std::chrono::high_resolution_clock::now();
}
