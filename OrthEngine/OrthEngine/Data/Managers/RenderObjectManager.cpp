#include "RenderObjectManager.hpp"

// ------------------------------------------------------------------------
RenderObjectManager::RenderObjectManager()
	: m_framebufferFactory(std::make_shared<FramebufferFactory>(FramebufferFactory::getInstance()))
	, m_renderObjectFactory(std::make_shared<RenderObjectFactory>(RenderObjectFactory::getInstance()))
	, m_shaderFactory(std::make_shared<ShaderFactory>(ShaderFactory::getInstance()))
	, m_renderObjectSceneCreator(std::make_shared<RenderObjectSceneCreator>())
	, m_framebuffer(m_framebufferFactory->getFramebuffer())
	, m_skyboxObject()
	, m_renderObjects()
	, m_fpsTextID(1)
	, m_playerIDs()
	, m_testFramebufferContents()
{
	LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
RenderObjectManager::~RenderObjectManager()
{
	LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void RenderObjectManager::initialize(const std::shared_ptr<FramebufferFactory> framebufferFactory, const std::shared_ptr<RenderObjectFactory> renderObjectFactory, const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<RenderObjectSceneCreator> renderObjectSceneCreator)
{
	// Only initialize factories if valid ones passed
	if (framebufferFactory != nullptr && renderObjectFactory != nullptr && shaderFactory != nullptr && renderObjectSceneCreator != nullptr)
	{
		m_framebufferFactory = framebufferFactory;
		m_renderObjectFactory = renderObjectFactory;
		m_shaderFactory = shaderFactory;
		m_renderObjectSceneCreator = renderObjectSceneCreator;
		m_framebuffer = framebufferFactory->getFramebuffer();

		LOG(INFO) << "test mocks set";
	}

	// Create reserved objects for tracking within manager
	ObjectConfig fpsTextConfig;
	fpsTextConfig.renderObjectProperties = { GeometryTypes::TEXT, false };
	fpsTextConfig.textProperties = { "", {10.0f, 775.0f}, 0.5f, {1.0f, 1.0f, 1.0f} };
	m_renderObjects.emplace(m_fpsTextID, m_renderObjectFactory->createRenderObject(fpsTextConfig).value());

	LOG(INFO) << "FPS object created";
}

// ------------------------------------------------------------------------
RenderObject* RenderObjectManager::createRenderObject(const unsigned int curID, const ObjectConfig& renderObjectConfig)
{
	// Attempt to create object and obtain value if exists
	std::optional<std::unique_ptr<RenderObject>> renderObjectOpt = m_renderObjectFactory->createRenderObject(renderObjectConfig);
	if (!renderObjectOpt.has_value())
		return nullptr;

	// Create render object pointer
	std::unique_ptr<RenderObject> renderObjectPtr = std::move(renderObjectOpt.value());
	RenderObject* renderObjectRet = renderObjectPtr.get();

	// Check if Skybox object, if so, store the shared_ptr in m_skyboxObject
	if (auto skyboxObj = dynamic_cast<SkyboxObject*>(renderObjectPtr.get()))
	{
		LOG(INFO) << "SkyboxObject updated";
		m_skyboxObject = std::make_unique<SkyboxObject>(*skyboxObj);
		return renderObjectRet;
	}

	// Create instanced objects if exists
	for (std::size_t index = 0; index < renderObjectConfig.instancedObjectProperties.textureIDs.size(); index++)
	{
		LOG(INFO) << "Instanced object created " << index;
		std::array<float, MathUtils::MAT4_SIZE> curModelMat = renderObjectConfig.instancedObjectProperties.modelMat.at(index);
		createInstancedObject(renderObjectPtr.get(), curModelMat, renderObjectConfig.instancedObjectProperties.textureIDs.at(index));
	}

	// Set object properties if needed
	if (renderObjectConfig.renderObjectProperties.geometryType == GeometryTypes::INSTANCED_POINT_LIGHT)
	{
		LOG(INFO) << "PointLight created";
		auto pointLight = dynamic_cast<PointLight*>(renderObjectPtr.get());
		updatePointLight(pointLight);
	}

	// Store the resulting object in the RenderObject list
	m_renderObjects.emplace(curID, std::move(renderObjectPtr));
	LOG(INFO) << "New object added " << curID;
	 
	// TODO: The player will now always get rendered either invisibility for first-person or a specified offset from the camera for third-person
	// Track as player if current object is a player
	if (renderObjectConfig.renderObjectProperties.isPlayer)
		m_playerIDs.push_back(curID);

	return renderObjectRet;
}

// ------------------------------------------------------------------------
void RenderObjectManager::updateFpsText(const std::string& newFpsString)
{
	auto it = m_renderObjects.find(m_fpsTextID);
	if (it != m_renderObjects.end())
	{
		auto fpsObj = dynamic_cast<TextObject*>(it->second.get());
		if (!fpsObj)
			return;

		TextProperties textProperties{ newFpsString, {10.0f, 775.0f}, 0.5f, {1.0f, 1.0f, 1.0f} };
		fpsObj->updateTextProperties(textProperties);
	}
}

// ------------------------------------------------------------------------
bool RenderObjectManager::updateRenderObject(const RenderObjectProperties& renderObjectProperties, const ObjectConfig& renderObjectConfig)
{
	// TODO: Implement this function once you determine what the EngineApp workflow will look like

	LOG(INFO) << "RenderObject updated";

	return false;
}

// ------------------------------------------------------------------------
void RenderObjectManager::createInstancedObject(RenderObject* renderObjectPtr, const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix, float textureID)
{
	// This conversion should always succeed because RenderObjectFactory should always return a InstancedObject type when GeometryTypes::INSTANCED_* passed
	InstancedObject* instancedLightPtr = dynamic_cast<InstancedObject*>(renderObjectPtr);
	LOG(INFO) << "Instanced object created";

	if (instancedLightPtr != nullptr)
		instancedLightPtr->addInstancedObject(modelMatrix, textureID);
	else
		assert(false);
}

// ------------------------------------------------------------------------
bool RenderObjectManager::removeInstancedObjectWithID(const unsigned int renderObjectID, const unsigned int instancedID)
{
	auto it = m_renderObjects.find(renderObjectID);
	LOG(INFO) << "Instanced object removed " << renderObjectID;

	if (it != m_renderObjects.end())
	{
		auto instObj = dynamic_cast<InstancedObject*>(it->second.get());
		if (!instObj)
			return false;

		instObj->removeInstancedObject(instancedID);
		return true;
	}
	else
	{
		return false;
	}
}

// ------------------------------------------------------------------------
void RenderObjectManager::removeObjectWithID(const unsigned int objectID)
{
	LOG(INFO) << "Remove object " << objectID;

	m_renderObjects.erase(objectID);
}

// ------------------------------------------------------------------------
void RenderObjectManager::updatePointLight(RenderObject* renderObjectPtr)
{
	// This conversion should always succeed because RenderObjectFactory should always return a PointLight when GeometryTypes::INSTANCED_POINT_LIGHT passed
	PointLight* pointLight = dynamic_cast<PointLight*>(renderObjectPtr);
	LOG(INFO) << "PointLight updated";

	if (pointLight != nullptr)
		pointLight->updateLightProperties();
	else
		assert(false);
}

// ------------------------------------------------------------------------
void RenderObjectManager::updateFramebufferSize(const int screenWidth, const int screenHeight)
{
	m_framebuffer->updateFramebufferSize(screenWidth, screenHeight, GlobalSettings::getInstance().getValue<bool>("msaaEnabled", false));
	m_framebuffer->updateRenderbufferSize(screenWidth, screenHeight);

	LOG(INFO) << "Framebuffer size updated";
}

// ------------------------------------------------------------------------
TransformationMatrices RenderObjectManager::getTransformationMatrices(const std::shared_ptr<Camera>& camera, const int windowWidth, const int windowHeight)
{
	// Compute view matrix from camera and render objects
	Transform transform{};
	transform.setPerspective(camera->GetZoom(), (float)windowWidth / (float)windowHeight, 0.1f, 100.0f);
	std::array<float, MathUtils::MAT4_SIZE> projectionMatrix = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::PROJECTION);
	std::array<float, MathUtils::MAT4_SIZE> viewMatrix = camera->GetViewMatrix();
	MathUtils::Vec3 frontMatrix = camera->GetFrontVector();

	LOG(INFO) << "Transformation matrix updated";

	return { projectionMatrix, viewMatrix, frontMatrix };
}

// ------------------------------------------------------------------------
void RenderObjectManager::renderAll(TransformationMatrices& transformationMatrices, const RenderLoopConfigOptions& renderLoopConfigOptions)
{
	// Pre-render loop setup
	renderLoopSetup(renderLoopConfigOptions.setupTeardownSettings);

	LOG(INFO) << "Render all objects";

	// Render SkyboxObject first if exists to prevent drawing on top of other objects
	if (m_skyboxObject.get() != nullptr)
		m_skyboxObject->render(transformationMatrices.projectionMatrix, transformationMatrices.viewMatrix);

	// Render all created objects
	for (const auto& pair : m_renderObjects)
	{
		// Update spotlight position based on player location
		if (auto spotlightObject = dynamic_cast<SpotLight*>(pair.second.get()))
			spotlightObject->updatePosition(renderLoopConfigOptions.lightProperties);

		// Update player object location to move with the camera
		auto it = std::find(m_playerIDs.begin(), m_playerIDs.end(), pair.first);
		if (it != m_playerIDs.end())
			dynamic_cast<NonInstancedObject*>(pair.second.get())->updateLocation(transformationMatrices.camPosition);

		// Render object
		pair.second->render(transformationMatrices.projectionMatrix, transformationMatrices.viewMatrix);
	}

	// Post render-loop rendering
	renderLoopCleanup(transformationMatrices, renderLoopConfigOptions.setupTeardownSettings);
}

// ------------------------------------------------------------------------
void RenderObjectManager::renderLoopSetup(const SetupTeardownSettings& setupTeardownSettings)
{
	LOG(INFO) << "Render loop setup";

	// Setup framebuffer
	if (setupTeardownSettings.framebufferEnabled)
	{
		if (setupTeardownSettings.msaaEnabled)
			m_framebuffer->bindFramebuffer(FramebufferTypes::MSAA);
		else
			m_framebuffer->bindFramebuffer(FramebufferTypes::POST);

		RasterizerUtility::setDepthTestState(true);
		Window::setWindowBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f }, true);
	}
}

// ------------------------------------------------------------------------
void RenderObjectManager::renderLoopCleanup(const TransformationMatrices& transformationMatrices, const SetupTeardownSettings& setupTeardownSettings)
{
	LOG(INFO) << "Render loop cleanup";

	// Render object normals
	if (setupTeardownSettings.drawNormals)
	{
		// Set normal shader params
		std::shared_ptr<Shader> normalShader = m_shaderFactory->getShader("Default_Normal").value();
		normalShader->use();
		normalShader->setMat4("projection", transformationMatrices.projectionMatrix.data());
		normalShader->setMat4("view", transformationMatrices.viewMatrix.data());

		std::shared_ptr<Shader> instancedNormalShader = m_shaderFactory->getShader("Default_InstancedNormal").value();
		instancedNormalShader->use();
		instancedNormalShader->setMat4("projection", transformationMatrices.projectionMatrix.data());
		instancedNormalShader->setMat4("view", transformationMatrices.viewMatrix.data());

		// Render all object normals
		for (const auto& pair : m_renderObjects)
		{
			if (NonInstancedObject* nonInstancedObjectPtr = dynamic_cast<NonInstancedObject*>(pair.second.get()))
				pair.second->drawNormals(normalShader, transformationMatrices.projectionMatrix, transformationMatrices.viewMatrix);

			if (InstancedObject* instancedObjectPtr = dynamic_cast<InstancedObject*>(pair.second.get()))
				pair.second->drawNormals(instancedNormalShader, transformationMatrices.projectionMatrix, transformationMatrices.viewMatrix);
		}
	}

	// Store test framebuffer if test render
	if (setupTeardownSettings.isTestFramebuffer)
	{
		m_testFramebufferContents = m_framebuffer->getFramebufferContents();
	}

	// Render framebuffer
	if (setupTeardownSettings.framebufferEnabled)
	{
		if (setupTeardownSettings.msaaEnabled)
			m_framebuffer->blitMSAA();

		m_framebuffer->bindFramebuffer(FramebufferTypes::DEFAULT);
		RasterizerUtility::setDepthTestState(false);
		Window::setWindowBackgroundColor({ 0.2f, 0.3f, 0.3f, 1.0f }, false);
		m_framebuffer->bindAndDrawQuadVAO();
	}
}

// ------------------------------------------------------------------------
std::vector<unsigned char> RenderObjectManager::getFramebufferContent()
{
	return m_testFramebufferContents;
}
