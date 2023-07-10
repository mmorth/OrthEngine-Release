#include "PointLight.hpp"

// ------------------------------------------------------------------------
PointLight::PointLight(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const std::shared_ptr<InstancedRasterizer> rasterizer, const ObjectMaterialProperties& objectMaterialProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams)
	: LightObject(shaderPtrs.at(0), rasterizer, phongLightProperties)
    , m_shaders(shaderPtrs)
    , m_objectMaterialProperties(objectMaterialProperties)
    , m_attenuationParams(attenuationParams)
{
    LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
PointLight::~PointLight()
{
    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void PointLight::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
    // Run texture and set material properties in the shader
    TextureMaps textureMaps = { m_objectMaterialProperties.diffuseMap, m_objectMaterialProperties.specularMap };
    m_rasterizer->activateTextures(textureMaps);
    m_shader->use();
    m_shader->setInt("material.diffuseMapArray", m_objectMaterialProperties.diffuseMap);
    m_shader->setInt("material.specularMap", m_objectMaterialProperties.specularMap);
    m_shader->setFloat("material.shininess", m_objectMaterialProperties.shininess);

    LightObject::render(projectionMatrix, viewMatrix);

    LOG(INFO) << "Render LightObject";
}

// ------------------------------------------------------------------------
void PointLight::updateLightProperties()
{
    // Update all shader's point light properties
    for (auto shader : m_shaders)
    {
        shader->use();
        shader->setInt("numPointLights", static_cast<int>(m_textureIDs.size()));
        TextureMaps textureMaps = { m_objectMaterialProperties.diffuseMap, m_objectMaterialProperties.specularMap };
        m_rasterizer->activateTextures(textureMaps);
        for (std::size_t index = 0; index < m_textureIDs.size(); index++)
        {
            std::string idx = std::to_string(index);
            shader->setVec3("pointLights[" + idx + "].position", std::vector<float>{m_modelMatrices.at(index)[12], m_modelMatrices.at(index)[13], m_modelMatrices.at(index)[14]}.data());
            shader->setVec3("pointLights[" + idx + "].ambient", &m_phongLightParams.ambient.x);
            shader->setVec3("pointLights[" + idx + "].diffuse", &m_phongLightParams.diffuse.x);
            shader->setVec3("pointLights[" + idx + "].specular", &m_phongLightParams.specular.x);
            shader->setFloat("pointLights[" + idx + "].constant", m_attenuationParams.constant);
            shader->setFloat("pointLights[" + idx + "].linear", m_attenuationParams.linear);
            shader->setFloat("pointLights[" + idx + "].quadratic", m_attenuationParams.quadratic);
        }
    }

    LOG(INFO) << "Update PointLight properties";
}

