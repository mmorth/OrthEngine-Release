#include "RenderObject.hpp"

// ------------------------------------------------------------------------
RenderObject::RenderObject(const std::shared_ptr<Shader> shaderPtr)
    : m_shader(shaderPtr)
{
    LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
RenderObject::~RenderObject()
{
    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void RenderObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
    // Render object with default object shader
    renderObject(m_shader, projectionMatrix, viewMatrix);

    LOG(INFO) << "Render RenderObject";
}

// ------------------------------------------------------------------------
void RenderObject::drawNormals(const std::shared_ptr<Shader> shaderPtr, std::array<float, MathUtils::MAT4_SIZE> projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix)
{
    // Render object with normal shader
    renderObject(shaderPtr, projectionMatrix, viewMatrix);

    LOG(INFO) << "Render RenderObject normals";
}

// ------------------------------------------------------------------------
void RenderObject::renderObject(const std::shared_ptr<Shader> shaderPtr, const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
    // Set the projection and view matrices
    shaderPtr->use();
    shaderPtr->setMat4("projection", projectionMatrix.data());
    shaderPtr->setMat4("view", viewMatrix.data());
}
