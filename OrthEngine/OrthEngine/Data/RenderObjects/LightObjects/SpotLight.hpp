#ifndef SPOTOBJECT_HPP
#define SPOTOBJECT_HPP

#include "RenderObjects/LightObjects/LightObjectStructs.hpp"
#include "RenderObjects/RenderObject.hpp"

class SpotLight : public RenderObject 
{
public:
    SpotLight(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams, const SpotlightCutoffParams& spotlightCutoffParams);
    ~SpotLight();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;

    virtual void updateSpotlightParams(const LightProperties& lightProperties, const PhongLightingParams& phongLightingParams, const AttenuationParams& attenuationParams, const SpotlightCutoffParams& spotlightCutoffParams);
    virtual void updatePosition(const LightProperties& lightProperties);

protected:
    std::vector<std::shared_ptr<Shader>> m_shaders;
    
};

#endif // SPOTLIGHT_HPP

