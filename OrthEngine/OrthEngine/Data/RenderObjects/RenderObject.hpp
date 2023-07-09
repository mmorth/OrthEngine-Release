#ifndef RENDEROBJECT_HPP
#define RENDEROBJECT_HPP

#include <array>

#include "MathUtils.hpp"
#include "Shader/Shader.hpp"

class RenderObject 
{
public:
    RenderObject(const std::shared_ptr<Shader> shaderPtr);
    ~RenderObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix);
    virtual void drawNormals(const std::shared_ptr<Shader> shaderPtr, std::array<float, MathUtils::MAT4_SIZE> projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix);

protected:
    std::shared_ptr<Shader> m_shader;

private:
    void renderObject(const std::shared_ptr<Shader> shaderPtr, const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix);

};

#endif // RENDEROBJECT_HPP
