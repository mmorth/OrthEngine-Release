#include "SkyboxRasterizer.hpp"

// ------------------------------------------------------------------------
SkyboxRasterizer::SkyboxRasterizer(const VertexData& vertexProperties)
    : Rasterizer(vertexProperties)
{
    //LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
SkyboxRasterizer::~SkyboxRasterizer()
{
    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
unsigned int SkyboxRasterizer::createNewVAO()
{
    // create skybox VAO
    unsigned int skyboxVAO;
    glGenVertexArrays(1, &skyboxVAO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_attribVBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    m_VAOs.push_back(skyboxVAO);

    //LOG(INFO) << "Create new Skybox VAO ID " << skyboxVAO;

    return skyboxVAO;
}

// ------------------------------------------------------------------------
void SkyboxRasterizer::drawArrays(const unsigned int VAO, const unsigned int numInstances)
{
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, m_numVertices);
    glBindVertexArray(0);

    glDepthFunc(GL_LESS);

    //LOG(INFO) << "Draw Skybox Arrays";
}
