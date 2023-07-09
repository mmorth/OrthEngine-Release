#ifndef RASTERIZERMOCK_HPP
#define RASTERIZERMOCK_HPP

#include "Rasterizers/Rasterizer.hpp"
#include "Rasterizers/InstancedRasterizer.hpp"
#include "Rasterizers/NonInstancedRasterizer.hpp"
#include "Rasterizers/SkyboxRasterizer.hpp"
#include "Rasterizers/TextRasterizer.hpp"
#include "Rasterizers/RasterizerUtility.hpp"

// ------------------------------------------------------------------------
class RasterizerMock : public Rasterizer 
{
public:
    MOCK_METHOD(unsigned int, createNewVAO, (), (override));
    MOCK_METHOD(void, drawArrays, (const unsigned int VAO, const unsigned int numInstances), (override));
    MOCK_METHOD(void, activateTextures, (const TextureMaps& textureMaps), (override));
    MOCK_METHOD(unsigned int, getNumVertices, (), (override));
    MOCK_METHOD(void, activateTexture, (const unsigned int textureID), (override));
};

// ------------------------------------------------------------------------
class InstancedRasterizerMock : public InstancedRasterizer 
{
public:
    InstancedRasterizerMock(const VertexData& vertexProperties, const std::vector<std::array<float, MathUtils::MAT4_SIZE>>& modelMatrices, const std::vector<float>& textureLayerIDs)
        : InstancedRasterizer(vertexProperties, modelMatrices, textureLayerIDs) {}

    MOCK_METHOD(unsigned int, createNewVAO, (), (override));
    MOCK_METHOD(void, drawArrays, (const unsigned int VAO, const unsigned int numInstances), (override));
    MOCK_METHOD(void, updateModelMatrices, ((const std::vector<std::array<float, MathUtils::MAT4_SIZE>>&) modelMatrices), (override));
    MOCK_METHOD(void, updateTextureLayerIDs, (const std::vector<float>& textureLayerIDs), (override));
    MOCK_METHOD(void, activateTextures, (const TextureMaps& textureMaps), (override));
};

// ------------------------------------------------------------------------
class NonInstancedRasterizerMock : public NonInstancedRasterizer 
{
public:
    NonInstancedRasterizerMock(const VertexData& vertexProperties)
        : NonInstancedRasterizer(vertexProperties) {}

    MOCK_METHOD(unsigned int, createNewVAO, (), (override));
    MOCK_METHOD(void, drawArrays, (const unsigned int VAO, const unsigned int numInstances), (override));
    MOCK_METHOD(void, activateTextures, (const TextureMaps& textureMaps), (override));
};

// ------------------------------------------------------------------------
class SkyboxRasterizerMock : public SkyboxRasterizer 
{
public:
    SkyboxRasterizerMock(const VertexData& vertexProperties)
        : SkyboxRasterizer(vertexProperties) {}

    MOCK_METHOD(unsigned int, createNewVAO, (), (override));
    MOCK_METHOD(void, drawArrays, (const unsigned int VAO, const unsigned int numInstances), (override));
    MOCK_METHOD(void, activateTextures, (const TextureMaps& textureMaps), (override));
};

class TextRasterizerMock : public TextRasterizer 
{
public:
    TextRasterizerMock(VertexData vertexProperties)
        : TextRasterizer(vertexProperties) {}

    MOCK_METHOD(unsigned int, createNewVAO, (const std::shared_ptr<Shader>& shader), (override));
    MOCK_METHOD(void, drawArrays, (const TextProperties& textProperties), (override));
    MOCK_METHOD(void, activateTextures, (const TextureMaps& textureMaps), (override));

    MOCK_METHOD(float, renderCharacter, (const char c, const TextProperties& textProperties, const float originalXPos), (override));
};

#endif // RASTERIZERMOCK_HPP
