#include "VertexDataFactory.hpp"

VertexDataFactory VertexDataFactory::instance;

// ------------------------------------------------------------------------
VertexDataFactory& VertexDataFactory::getInstance()
{
    return instance;
}

// ------------------------------------------------------------------------
VertexDataFactory::VertexDataFactory()
{
    // Cube Object
    VertexData cubeVertexData;
    cubeVertexData.vertexAttributes = {
        // positions          // normals           // texture coords
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        // left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        // right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         // bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        // top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    cubeVertexData.attributesPerVertex = 8;
    m_vertexMap["Default_Cube"] = cubeVertexData;

    // Plane vertex attributes
    VertexData planeVertexData;
    planeVertexData.vertexAttributes = {
        // positions         // normals           // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f
    };
    planeVertexData.attributesPerVertex = 8;
    m_vertexMap["Default_Plane"] = planeVertexData;

    // Quad Vertex Attributes
    VertexData quadVertexData;
    quadVertexData.vertexAttributes = {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    quadVertexData.attributesPerVertex = 4;
    m_vertexMap["Default_Quad"] = quadVertexData;

    // Skybox Vertex Attributes
    VertexData skyboxVertexData;
    skyboxVertexData.vertexAttributes = {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    skyboxVertexData.attributesPerVertex = 3;
    m_vertexMap["Default_Skybox"] = skyboxVertexData;

    LOG(INFO) << "Default VertexData created";
}

// ------------------------------------------------------------------------
void VertexDataFactory::addVertexData(const std::string& key, const VertexData& vertexData)
{
    m_vertexMap.emplace(key, vertexData);

    LOG(INFO) << "Vertex data added: " << key;
}

// ------------------------------------------------------------------------
std::optional<VertexData> VertexDataFactory::getVertexData(const std::string& key)
{
    auto it = m_vertexMap.find(key);
    LOG(INFO) << "Vertex data retrieved " << key;

    if (it != m_vertexMap.end())
        return it->second;
    else
        return std::nullopt;
}
