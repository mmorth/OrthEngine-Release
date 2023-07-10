#include "SpotLight.hpp"

// ------------------------------------------------------------------------
SpotLight::SpotLight(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams, const SpotlightCutoffParams& spotlightCutoffParams)
	: RenderObject(shaderPtrs.at(0))
	, m_shaders(shaderPtrs)
{
	LOG(INFO) << "ctor";

	updateSpotlightParams(lightProperties, phongLightProperties, attenuationParams, spotlightCutoffParams);
}

// ------------------------------------------------------------------------
SpotLight::~SpotLight()
{
	LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void SpotLight::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{}

// ------------------------------------------------------------------------
void SpotLight::updateSpotlightParams(const LightProperties& lightProperties, const PhongLightingParams& phongLightingParams, const AttenuationParams& attenuationParams, const SpotlightCutoffParams& spotlightCutoffParams)
{
	for (auto shader : m_shaders)
	{
		shader->use();
		shader->setVec3("spotLight.position", &lightProperties.position.x);
		shader->setVec3("spotLight.direction", &lightProperties.direction.x);
		shader->setVec3("spotLight.ambient", &phongLightingParams.ambient.x);
		shader->setVec3("spotLight.diffuse", &phongLightingParams.diffuse.x);
		shader->setVec3("spotLight.specular", &phongLightingParams.specular.x);
		shader->setFloat("spotLight.constant", attenuationParams.constant);
		shader->setFloat("spotLight.linear", attenuationParams.linear);
		shader->setFloat("spotLight.quadratic", attenuationParams.quadratic);
		shader->setFloat("spotLight.innerCutOff", glm::cos(glm::radians(spotlightCutoffParams.innerCutoff)));
		shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(spotlightCutoffParams.outerCutoff)));
	}

	LOG(INFO) << "Update spotlight properties";
}

// ------------------------------------------------------------------------
void SpotLight::updatePosition(const LightProperties& lightProperties)
{
	for (auto shader : m_shaders)
	{
		shader->use();
		shader->setVec3("spotLight.position", &lightProperties.position.x);
		shader->setVec3("spotLight.direction", &lightProperties.direction.x);
	}

	LOG(INFO) << "Update spotlight position";
}

