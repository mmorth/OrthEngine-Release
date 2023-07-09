#ifndef POINTLIGHT_HPP
#define POINTLIGHT_HPP

#include "RenderObjects/LightObjects/LightObject.hpp"
#include "RenderObjects/GeometricObjects/GeometricObjectStructs.hpp"

class PointLight : public LightObject 
{
public:
    PointLight(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const std::shared_ptr<InstancedRasterizer> rasterizer, const ObjectMaterialProperties& objectMaterialProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams);
    ~PointLight();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;

    virtual void updateLightProperties();

protected:
    std::vector<std::shared_ptr<Shader>> m_shaders;

    ObjectMaterialProperties m_objectMaterialProperties;
    AttenuationParams m_attenuationParams;

};

#endif // POINTLIGHT_HPP
