#ifndef SKYBOXOBJECT_HPP
#define SKYBOXOBJECT_HPP

#include "RenderObjects/RenderObject.hpp"
#include "Rasterizers/SkyboxRasterizer.hpp"

class SkyboxObject : public RenderObject 
{
public:
    SkyboxObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<SkyboxRasterizer> skyboxRasterizerPtr, const unsigned int textureID);
    ~SkyboxObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix);

protected:
    std::shared_ptr<SkyboxRasterizer> m_rasterizer;

    unsigned int m_textureID;
    unsigned int m_VAO;
};

#endif // SKYBOXOBJECT_HPP
