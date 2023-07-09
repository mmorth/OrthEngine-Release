#include "Rasterizer.hpp"

// ------------------------------------------------------------------------
Rasterizer::Rasterizer(const VertexData& vertexProperties)
    : m_numVertices(vertexProperties.vertexAttributes.size() / vertexProperties.attributesPerVertex)
    , m_VAOs()
    , m_attribVBO(0)
{
    // Generate new VBO for Rasterizer
    glGenBuffers(1, &m_attribVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_attribVBO);
    glBufferData(GL_ARRAY_BUFFER, vertexProperties.vertexAttributes.size() * sizeof(float), vertexProperties.vertexAttributes.data(), GL_STATIC_DRAW);

    //LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
Rasterizer::~Rasterizer()
{
    // Deallocate buffers and arrays after no longer used
    glDeleteBuffers(1, &m_attribVBO);
    for (unsigned int VAO : m_VAOs)
        glDeleteVertexArrays(1, &VAO);

    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
unsigned int Rasterizer::createNewVAO()
{
    // Generate new VAO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    // Bind VBO and new VAO and define vertex attributes
    glBindBuffer(GL_ARRAY_BUFFER, m_attribVBO);
    glBindVertexArray(VAO);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coordinates attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Add new VAO to m_VAOs
    m_VAOs.push_back(VAO);

    //LOG(INFO) << "New rasterizer VAO ID " << VAO;

    return VAO;
}

// ------------------------------------------------------------------------
unsigned int Rasterizer::getNumVertices()
{
    return m_numVertices;
}

// ------------------------------------------------------------------------
void Rasterizer::activateTextures(const TextureMaps& textureMaps)
{
    // Activate the diffuse map if not null
    if (textureMaps.diffuseMap != 0)
        activateTexture(textureMaps.diffuseMap);

    // Activate specular map if not null
    if (textureMaps.specularMap != 0)
        activateTexture(textureMaps.specularMap);

    //LOG(INFO) << "Textures activated: Diffuse ID " << textureMaps.diffuseMap << " , Specular ID " << textureMaps.specularMap;
}

// ------------------------------------------------------------------------
void Rasterizer::activateTexture(const unsigned int textureID)
{
    glActiveTexture(GL_TEXTURE0 + textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
}

// ------------------------------------------------------------------------
void Rasterizer::drawArrays(const unsigned int VAO, const unsigned int numInstances)
{}
