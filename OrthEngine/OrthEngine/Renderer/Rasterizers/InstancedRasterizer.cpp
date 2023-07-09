#include "InstancedRasterizer.hpp"

// ------------------------------------------------------------------------
InstancedRasterizer::InstancedRasterizer(const VertexData& vertexProperties, const std::vector<std::array<float, MathUtils::MAT4_SIZE>>& modelMatrices, const std::vector<float>& textureLayerIDs)
    : Rasterizer(vertexProperties)
    , m_instancedModelMatrixVBO(0)
    , m_instancedTextureVBO(0)
{
    //LOG(INFO) << "ctor";
    
    // Create the instanced model matrix
    glGenBuffers(1, &m_instancedModelMatrixVBO);
    updateModelMatrices(modelMatrices);

    // Create the instanced texture
    glGenBuffers(1, &m_instancedTextureVBO);
    updateTextureLayerIDs(textureLayerIDs);
}

// ------------------------------------------------------------------------
InstancedRasterizer::~InstancedRasterizer()
{
    // Deallocate buffers and arrays after no longer used
    glDeleteBuffers(1, &m_instancedModelMatrixVBO);
    glDeleteBuffers(1, &m_instancedTextureVBO);

    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void InstancedRasterizer::updateModelMatrices(const std::vector<std::array<float, MathUtils::MAT4_SIZE>>& modelMatrices)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedModelMatrixVBO);
    glBufferData(GL_ARRAY_BUFFER, MathUtils::MAT4_SIZE * modelMatrices.size() * sizeof(float), modelMatrices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //LOG(INFO) << "Update instanced model matrix VBO " << m_instancedModelMatrixVBO;
}

// ------------------------------------------------------------------------
void InstancedRasterizer::updateTextureLayerIDs(const std::vector<float>& textureLayerIDs)
{
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedTextureVBO);
    glBufferData(GL_ARRAY_BUFFER, textureLayerIDs.size() * sizeof(float), textureLayerIDs.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    //LOG(INFO) << "Update instanced texture VBO " << m_instancedTextureVBO;
}

// ------------------------------------------------------------------------
unsigned int InstancedRasterizer::createNewVAO()
{
    // Create attrib VAO params
    unsigned int VAO = Rasterizer::createNewVAO();

    // Set the instanced attributes (Mat4 representing Model matrix transformation
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedModelMatrixVBO);
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, MathUtils::MAT4_SIZE * sizeof(float), (void*)0);
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, MathUtils::MAT4_SIZE * sizeof(float), (void*)(MathUtils::VEC4_SIZE * sizeof(float)));
    glEnableVertexAttribArray(5);
    glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, MathUtils::MAT4_SIZE * sizeof(float), (void*)(2 * MathUtils::VEC4_SIZE * sizeof(float)));
    glEnableVertexAttribArray(6);
    glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, MathUtils::MAT4_SIZE * sizeof(float), (void*)(3 * MathUtils::VEC4_SIZE * sizeof(float)));

    // Set the attribute pointers for the texture indices
    glBindBuffer(GL_ARRAY_BUFFER, m_instancedTextureVBO);
    glEnableVertexAttribArray(7);
    glVertexAttribPointer(7, 1, GL_FLOAT, GL_FALSE, sizeof(float), (void*)0);

    // apply on per-instance basis
    glVertexAttribDivisor(3, 1);
    glVertexAttribDivisor(4, 1);
    glVertexAttribDivisor(5, 1);
    glVertexAttribDivisor(6, 1);
    glVertexAttribDivisor(7, 1);

    //LOG(INFO) << "Create instanced VAO ID " << VAO;

    return VAO;
}

// ------------------------------------------------------------------------
void InstancedRasterizer::drawArrays(const unsigned int VAO, const unsigned int numInstances)
{
    glBindVertexArray(VAO);
    glDrawArraysInstanced(GL_TRIANGLES, 0, m_numVertices, numInstances);

    //LOG(INFO) << "Draw Instanced rasterizer arrays";
}
