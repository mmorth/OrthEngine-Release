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
	, m_physicsManager(std::make_shared<PhysicsManager>())
	, m_camera(CameraFactory::getInstance().getCamera(CameraTypes::THIRD_PERSON, MathUtils::Vec3{ 0.0f, 1.0f, 3.0f }))
	, m_deltaTime(0.0f)
	, m_lastFrame(0.0f)
	, m_isTestRender(isTestRender)
	, m_renderObjectSceneCreator(std::make_shared<RenderObjectSceneCreator>())
	, m_curID(RESERVED_IDS+1)
{
	LOG(INFO) << "ctor";

	// Initialize scene objects and collisions
	loadScene(m_renderObjectSceneCreator->createExperimentalScene());

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
void EngineApp::initialize(const std::shared_ptr<Window> window, const std::shared_ptr<RenderObjectManager> renderObjectManager, const std::shared_ptr<PhysicsManager> physicsManager, const std::shared_ptr<Camera> camera, const std::shared_ptr<RenderObjectSceneCreator> renderObjectSceneCreator)
{
	m_window = window;
	m_renderObjectManager = renderObjectManager;
	m_physicsManager = physicsManager;
	m_camera = camera;
	m_renderObjectSceneCreator = renderObjectSceneCreator;

	LOG(INFO) << "EngineApp mocks set";
}

// ------------------------------------------------------------------------
void EngineApp::loadScene(const std::vector<ObjectConfig>& renderObjectConfigs)
{
	m_renderObjectManager->initialize();

	// Create all the objets specified by the scene
	for (ObjectConfig renderObjectConfig : renderObjectConfigs)
	{
		m_renderObjectManager->createRenderObject(m_curID, renderObjectConfig);
		
		if (renderObjectConfig.renderObjectProperties.isCollidable)
			m_physicsManager->createRigidBody(m_curID, renderObjectConfig, renderObjectConfig.renderObjectProperties.isPlayer);
		m_curID++;
	}
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

		// Render objects and process collisions
		RenderLoopConfigOptions renderLoopConfigOptions = { {m_camera->GetPosition(), m_camera->GetFront()}, {}, 
			{GlobalSettings::getInstance().getValue<bool>("framebufferEnabled", false), GlobalSettings::getInstance().getValue<bool>("msaaEnabled", false)
			, GlobalSettings::getInstance().getValue<bool>("drawNormals", false), m_isTestRender} };
		m_physicsManager->updatePlayerLocation(m_camera->GetFrontVector(), m_deltaTime);

		// Step simulation and detect bullet collisions and update object positions
		m_physicsManager->stepSimulationAndCheckCollision(m_deltaTime);
		MathUtils::Vec3 newPlayerPosition = m_physicsManager->getPlayerLocation();
		m_camera->SetTarget(newPlayerPosition);
		// Detect and handle camera collision
		MathUtils::Vec3 originalPosition = m_camera->GetFrontVector();
		MathUtils::Vec3 newCamPosition = m_physicsManager->detectCameraCollision(newPlayerPosition, m_camera->GetPosition(), m_camera->GetFront());
		m_camera->SetPosition(newCamPosition);
		TransformationMatrices transformationMatrices = m_renderObjectManager->getTransformationMatrices(m_camera, windowWidth, windowHeight);
		transformationMatrices.camPosition = originalPosition;
		// Render all objects based on detected collisions
		m_renderObjectManager->renderAll(transformationMatrices, renderLoopConfigOptions);

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
