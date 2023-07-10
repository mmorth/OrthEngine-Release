#include "SkyboxObject.hpp"

// ------------------------------------------------------------------------
SkyboxObject::SkyboxObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<SkyboxRasterizer> skyboxRasterizerPtr, const unsigned int textureID)
    : RenderObject(shaderPtr)
    , m_rasterizer(skyboxRasterizerPtr)
    , m_textureID(textureID)
    , m_VAO(m_rasterizer->createNewVAO())
{
    LOG(INFO) << "ctor";

    // load skybox
    m_shader->use();
    m_shader->setInt("skybox", 0);
}

// ------------------------------------------------------------------------
SkyboxObject::~SkyboxObject() {}

//// ------------------------------------------------------------------------
//void SkyboxObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
//{
//    // convert skybox projection matrix to mat3 to avoid translation and depth changing values
//    viewMatrix[3] = 0;
//    viewMatrix[7] = 0;
//    viewMatrix[11] = 0;
//    viewMatrix[12] = 0;
//    viewMatrix[13] = 0;
//    viewMatrix[14] = 0;
//    viewMatrix[15] = 0;
//
//    // set view and projection matrix
//    RenderObject::render(projectionMatrix, viewMatrix);
//
//    // Set skybox texture
//    TextureMaps textureMaps = { m_textureID, 0 };
//    m_rasterizer->activateTextures(textureMaps);
//
//    // render skybox cube
//    m_rasterizer->drawArrays(m_VAO);
//
//    LOG(INFO) << "Render SkyboxObject";
//}

// ------------------------------------------------------------------------
void SkyboxObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix)
{
    // convert skybox projection matrix to mat3 to avoid translation and depth changing values
    viewMatrix[3] = 0;
    viewMatrix[7] = 0;
    viewMatrix[11] = 0;
    viewMatrix[12] = 0;
    viewMatrix[13] = 0;
    viewMatrix[14] = 0;
    viewMatrix[15] = 0;

    // set view and projection matrix
    RenderObject::render(projectionMatrix, viewMatrix);

    // Set skybox texture
    TextureMaps textureMaps = { m_textureID, 0 };
    m_rasterizer->activateTextures(textureMaps);

    // render skybox cube
    m_rasterizer->drawArrays(m_VAO);

    LOG(INFO) << "Render SkyboxObject";
}
