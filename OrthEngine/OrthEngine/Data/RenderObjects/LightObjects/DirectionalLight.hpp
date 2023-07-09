#ifndef DIRECTIONALLIGHT_HPP
#define DIRECTIONALLIGHT_HPP

#include "LightObjectStructs.hpp"
#include "RenderObjects/RenderObject.hpp"

class DirectionalLight : public RenderObject 
{
public:
    DirectionalLight(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const LightProperties& lightProperties, const PhongLightingParams& phongLightingParams);
    ~DirectionalLight();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;
    virtual void setDirectionLightParams(const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties);

protected:
    std::vector<std::shared_ptr<Shader>> m_shaders;

};

#endif // DIRECTIONALLIGHT_HPP
