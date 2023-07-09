#ifndef INSTANCEDRASTERIZER_HPP
#define INSTANCEDRASTERIZER_HPP

#include <array>
#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "MathUtils.hpp"
#include "Rasterizer.hpp"

class InstancedRasterizer : public Rasterizer 
{
public:

    InstancedRasterizer(const VertexData& vertexProperties, const std::vector<std::array<float, MathUtils::MAT4_SIZE>>& modelMatrices, const std::vector<float>& textureLayerIDs);
    ~InstancedRasterizer();

    virtual unsigned int createNewVAO() override;
    virtual void drawArrays(const unsigned int VAO, const unsigned int numInstances = 1) override;

    virtual void updateModelMatrices(const std::vector<std::array<float, MathUtils::MAT4_SIZE>>& modelMatrices);
    virtual void updateTextureLayerIDs(const std::vector<float>& textureLayerIDs);

protected:
    unsigned int m_instancedModelMatrixVBO;
    unsigned int m_instancedTextureVBO;
};

#endif // INSTANCEDRASTERIZER_HPP
