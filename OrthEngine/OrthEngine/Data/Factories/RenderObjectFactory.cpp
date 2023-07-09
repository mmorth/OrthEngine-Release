#include "RenderObjectFactory.hpp"

// ------------------------------------------------------------------------
RenderObjectFactory& RenderObjectFactory::getInstance()
{
	static RenderObjectFactory instance;
	return instance;
}

// ------------------------------------------------------------------------
RenderObjectFactory::RenderObjectFactory()
	: m_shaderFactory(std::make_shared<ShaderFactory>(ShaderFactory::getInstance()))
	, m_rasterizerFactory(std::make_shared<RasterizerFactory>(RasterizerFactory::getInstance()))
	, m_textureFactory(std::make_shared<TextureFactory>())
{
	//LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
void RenderObjectFactory::initialize(const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<RasterizerFactory> rasterizerFactory, const std::shared_ptr<TextureFactory> textureFactory)
{
	m_shaderFactory = shaderFactory;
	m_rasterizerFactory = rasterizerFactory;
	m_textureFactory = textureFactory;

	//LOG(INFO) << "RenderObjectFactory mock variables updated";
}

// ------------------------------------------------------------------------
std::optional<std::unique_ptr<RenderObject>> RenderObjectFactory::createRenderObject(const RenderObjectConfig& renderObjectConfig)
{
	std::unique_ptr<RenderObject> renderObjectPtr;

	switch (renderObjectConfig.renderObjectProperties.geometryType)
	{
		case GeometryTypes::INSTANCED_CUBE:
		{
			ObjectMaterialProperties objectMaterialProperties = { renderObjectConfig.objectMaterialNames.shininess, m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.diffuseMap).value(), m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.specularMap).value() };
			renderObjectPtr = std::make_unique<GeometricInstancedObject>(GeometricInstancedObject(m_shaderFactory->getShader("Default_InstancedObject").value(), std::static_pointer_cast<InstancedRasterizer>(m_rasterizerFactory->getRasterizer(renderObjectConfig.renderObjectProperties.geometryType).value()), objectMaterialProperties));
			//LOG(INFO) << "Instanced cube created";
			break;
		}
		case GeometryTypes::NONINSTANCED_CUBE:
		{
			ObjectMaterialProperties objectMaterialProperties = { renderObjectConfig.objectMaterialNames.shininess, m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.diffuseMap).value(), m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.specularMap).value() };
			renderObjectPtr = std::make_unique<GeometricNonInstancedObject>(GeometricNonInstancedObject(m_shaderFactory->getShader("Default_Object").value(), std::static_pointer_cast<NonInstancedRasterizer>(m_rasterizerFactory->getRasterizer(renderObjectConfig.renderObjectProperties.geometryType).value()), renderObjectConfig.objectLocation, objectMaterialProperties));
			//LOG(INFO) << "NonInstanced cube created";
			break;
		}
		case GeometryTypes::NONINSTANCED_PLANE:
		{
			ObjectMaterialProperties objectMaterialProperties = { renderObjectConfig.objectMaterialNames.shininess, m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.diffuseMap).value(), m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.specularMap).value() };
			renderObjectPtr = std::make_unique<GeometricNonInstancedObject>(GeometricNonInstancedObject(m_shaderFactory->getShader("Default_Object").value(), std::static_pointer_cast<NonInstancedRasterizer>(m_rasterizerFactory->getRasterizer(renderObjectConfig.renderObjectProperties.geometryType).value()), renderObjectConfig.objectLocation, objectMaterialProperties));
			//LOG(INFO) << "NonInstanced plane created";
			break;
		}
		case GeometryTypes::SKYBOX:
		{
			renderObjectPtr = std::make_unique<SkyboxObject>(SkyboxObject(m_shaderFactory->getShader("Default_Skybox").value(), std::static_pointer_cast<SkyboxRasterizer>(m_rasterizerFactory->getRasterizer(renderObjectConfig.renderObjectProperties.geometryType).value()), m_textureFactory->getTextureID("Default_Skybox").value()));
			//LOG(INFO) << "Skybox created";
			break;
		}
		case GeometryTypes::TEXT:
		{
			renderObjectPtr = std::make_unique<TextObject>(TextObject(m_shaderFactory->getShader("Default_Text").value(), std::static_pointer_cast<TextRasterizer>(m_rasterizerFactory->getRasterizer(renderObjectConfig.renderObjectProperties.geometryType).value()), renderObjectConfig.textProperties));
			auto textObj = dynamic_cast<TextObject*>(renderObjectPtr.get());
			textObj->updateTextProperties(renderObjectConfig.textProperties);
			//LOG(INFO) << "Text created";
			break;
		}
		case GeometryTypes::INSTANCED_POINT_LIGHT:
		{
			std::vector<std::shared_ptr<Shader>> shaderPtrs{ m_shaderFactory->getShader("Default_InstancedObject").value(), m_shaderFactory->getShader("Default_Object").value() };
			ObjectMaterialProperties objectMaterialProperties = { renderObjectConfig.objectMaterialNames.shininess, m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.diffuseMap).value(), m_textureFactory->getTextureID(renderObjectConfig.objectMaterialNames.specularMap).value() };
			renderObjectPtr = std::make_unique<PointLight>(PointLight(shaderPtrs, std::static_pointer_cast<InstancedRasterizer>(m_rasterizerFactory->getRasterizer(renderObjectConfig.renderObjectProperties.geometryType).value()), objectMaterialProperties, renderObjectConfig.phongLightProperties, renderObjectConfig.attenuationParams));
			//LOG(INFO) << "Instanced PointLight created";
			break;
		}
		case GeometryTypes::INSTANCED_SPOT_LIGHT:
		{
			// TODO: Update this to be an instanced version where the spotlight is not always following the player
			std::vector<std::shared_ptr<Shader>> shaderPtrs{ m_shaderFactory->getShader("Default_InstancedObject").value(), m_shaderFactory->getShader("Default_Object").value() };
			renderObjectPtr = std::make_unique<SpotLight>(SpotLight(shaderPtrs, renderObjectConfig.lightProperties, renderObjectConfig.phongLightProperties, renderObjectConfig.attenuationParams, renderObjectConfig.spotlightCutoffParams));
			//LOG(INFO) << "Instanced Spot Light created";
			break;
		}
		case GeometryTypes::DIRECTIONAL_LIGHT:
		{
			std::vector<std::shared_ptr<Shader>> shaderPtrs{ m_shaderFactory->getShader("Default_InstancedObject").value(), m_shaderFactory->getShader("Default_Object").value() };
			renderObjectPtr = std::make_unique<DirectionalLight>(DirectionalLight(shaderPtrs, renderObjectConfig.lightProperties, renderObjectConfig.phongLightProperties));
			//LOG(INFO) << "DirectionalLight created";
			break;
		}
		// TODO: Create implementation for rendering an instanced plane and a quad
		case GeometryTypes::INSTANCED_PLANE:
		case GeometryTypes::QUAD:
		default:
			//LOG(INFO) << "No implementation for " << geometricType;
			return std::nullopt;
	}

	return renderObjectPtr;
}
