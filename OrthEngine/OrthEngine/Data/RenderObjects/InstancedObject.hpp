#include "Rasterizers/InstancedRasterizer.hpp"
#include "RenderObject.hpp"

#ifndef INSTANCEDOBJECT_HPP
#define INSTANCEDOBJECT_HPP

class InstancedObject : public RenderObject 
{
public:
    InstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer);
    ~InstancedObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;
    virtual void drawNormals(const std::shared_ptr<Shader> shaderPtr, std::array<float, MathUtils::MAT4_SIZE> projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix) override;

    virtual void addInstancedObject(const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix, const float textureID);
    virtual void removeInstancedObject(const unsigned int index);

protected:
    std::shared_ptr<InstancedRasterizer> m_rasterizer;

    unsigned int m_VAO;
    std::vector<std::array<float, MathUtils::MAT4_SIZE>> m_modelMatrices;
    std::vector<float> m_textureIDs;

private:
    void renderObject(const std::shared_ptr<Shader> shaderPtr, const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix);

};

#endif // INSTANCEDOBJECT_HPP
