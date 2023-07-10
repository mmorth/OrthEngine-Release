#include "DirectionalLight.hpp"

// ------------------------------------------------------------------------
DirectionalLight::DirectionalLight(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties)
	: RenderObject(shaderPtrs.at(0))
	, m_shaders(shaderPtrs)
{
	LOG(INFO) << "ctor";

	setDirectionLightParams(lightProperties, phongLightProperties);
}

// ------------------------------------------------------------------------
DirectionalLight::~DirectionalLight()
{
	LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void DirectionalLight::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{}

// ------------------------------------------------------------------------
void DirectionalLight::setDirectionLightParams(const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties)
{
	for (auto shader : m_shaders)
	{
		shader->use();
		shader->setVec3("directionalLight.direction", &lightProperties.direction.x);
		shader->setVec3("directionalLight.ambient", &phongLightProperties.ambient.x);
		shader->setVec3("directionalLight.diffuse", &phongLightProperties.diffuse.x);
		shader->setVec3("directionalLight.specular", &phongLightProperties.specular.x);
	}

	LOG(INFO) << "Set DirectionalLight params";
}