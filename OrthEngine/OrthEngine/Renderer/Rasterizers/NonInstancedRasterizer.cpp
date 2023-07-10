#include "NonInstancedRasterizer.hpp"

// ------------------------------------------------------------------------
NonInstancedRasterizer::NonInstancedRasterizer(const VertexData& vertexProperties)
    : Rasterizer(vertexProperties)
{
    LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
NonInstancedRasterizer::~NonInstancedRasterizer()
{
    Rasterizer::~Rasterizer();

    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
unsigned int NonInstancedRasterizer::createNewVAO()
{
    return Rasterizer::createNewVAO();
}

// ------------------------------------------------------------------------
void NonInstancedRasterizer::drawArrays(const unsigned int VAO, const unsigned int numInstances)
{
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);

    LOG(INFO) << "Draw NonInstanced Rasterizer Arrays";
}
