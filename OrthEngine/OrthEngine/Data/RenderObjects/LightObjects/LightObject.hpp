#ifndef LIGHTOBJECT_HPP
#define LIGHTOBJECT_HPP

#include "LightObjectStructs.hpp"
#include "RenderObjects/InstancedObject.hpp"

class LightObject : public InstancedObject 
{
public:
    LightObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer, const PhongLightingParams& phongLightProperties);
    ~LightObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;

protected:
    PhongLightingParams m_phongLightParams;
    
};

#endif // LIGHTOBJECT_HPP
