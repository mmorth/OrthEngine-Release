#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Shader/tests/ShaderMock.hpp"
#include "OpenGLFixture.hpp"

#include "Rasterizers/Rasterizer.hpp"
#include "Rasterizers/InstancedRasterizer.hpp"
#include "Rasterizers/NonInstancedRasterizer.hpp"
#include "Rasterizers/SkyboxRasterizer.hpp"
#include "Rasterizers/TextRasterizer.hpp"
#include "Rasterizers/RasterizerUtility.hpp"

namespace
{
    // ------------------------------------------------------------------------
    std::array<float, MathUtils::MAT4_SIZE> IDENTITY_MATRIX
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };

    template <typename T>
    void verifyCreateRasterizerProperlySetsMemberVariables(::testing::StrictMock<T>& rasterizerTestable, VertexData vertexData)
    {
        // FUT = constructor
        EXPECT_TRUE(rasterizerTestable.getAttribVBO() > 0);
        EXPECT_EQ(rasterizerTestable.getNumVertices(), vertexData.vertexAttributes.size() / vertexData.attributesPerVertex);

        glBindBuffer(GL_ARRAY_BUFFER, rasterizerTestable.getAttribVBO());
        GLint bufferSize; glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
        EXPECT_EQ(bufferSize, sizeof(float) * vertexData.vertexAttributes.size());
    }

    template <typename T>
    void verifyCreateVAOPropertlySetsVertexAttributes(::testing::StrictMock<T>& rasterizerTestable, VertexData vertexData, unsigned int newVAO, unsigned int enabledChildAttributes)
    {
        glBindVertexArray(newVAO);
        EXPECT_TRUE(newVAO > 0);

        // Confirm VAO attributes
        // Get the number of vertex attributes
        GLint numVertexAttribs;
        glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttribs);

        // Iterate over each vertex attribute
        for (GLint attribIndex = 0; attribIndex < numVertexAttribs; ++attribIndex)
        {
            GLint enabled; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
            if (attribIndex < static_cast<GLint>(vertexData.attributesPerVertex))
            {
                // Obtain vertex attribute data
                GLint size; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
                GLenum type; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_TYPE, reinterpret_cast<GLint*>(&type));
                GLint stride; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);

                // Verify data based on attribute index
                EXPECT_EQ(true, enabled);
                GLint expectedSize = (attribIndex == 2) ? 2 : 3;
                EXPECT_EQ(expectedSize, size);
                EXPECT_EQ(GL_FLOAT, type);
                EXPECT_EQ(8 * sizeof(float), stride);
            }
            else if (attribIndex < static_cast<GLint>(enabledChildAttributes))
            {
                EXPECT_EQ(true, enabled);
            }
            else
            {
                // All other vertex attribs should be disabled
                EXPECT_EQ(false, enabled);
            }
        }

        // Ensure VAO is added to VAOs list
        std::vector<unsigned int> VAOs = rasterizerTestable.getVAOs();
        EXPECT_EQ(1, VAOs.size());
        EXPECT_EQ(VAOs.at(0), newVAO);
    }

    // ------------------------------------------------------------------------
    void verifyVertexAttributes(unsigned int attribIndex, bool attribCheckCondition, GLint expectedSize, GLenum expectedType, GLint expectedStride, GLint expectedDivisor)
    {
        GLint enabled; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);

        if (attribCheckCondition)
        {
            // Obtain vertex attribute data
            GLint size; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);
            GLenum type; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_TYPE, reinterpret_cast<GLint*>(&type));
            GLint stride; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);
            GLint divisor; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &divisor);

            // Verify data based on attribute index
            EXPECT_EQ(true, enabled);
            EXPECT_EQ(expectedSize, size);
            EXPECT_EQ(expectedType, type);
            EXPECT_EQ(expectedStride, stride);
            EXPECT_EQ(expectedDivisor, divisor);
        }
    }
}

// ===============================================================
// RasterizerTestable
// ===============================================================
class RasterizerTestable : public Rasterizer {
public:
    RasterizerTestable(VertexData vertexProperties)
        : Rasterizer(vertexProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    unsigned int getAttribVBO() { return m_attribVBO; }
    std::vector<unsigned int> getVAOs() { return m_VAOs; }

    // ===============================================================
    // Mock Methods
    // ===============================================================
    MOCK_METHOD(void, activateTexture, (unsigned int textureID), ());

    // ===============================================================
    // Constants
    // ===============================================================
    static constexpr unsigned int VALID_DIFFUSE_TEXTURE_ID = 1;
    static constexpr unsigned int VALID_SPECULAR_TEXTURE_ID = 2;
    static constexpr unsigned int INVALID_TEXTURE_ID = 0;
};

// ===============================================================
// RasterizerTest
// ===============================================================
class RasterizerTest : public OpenGLTestFixture
{
public:
    static VertexData s_vertexProperties;
    static ::testing::StrictMock<RasterizerTestable>* s_rasterizerTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_vertexProperties = { { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        s_rasterizerTestable = new ::testing::StrictMock<RasterizerTestable>(s_vertexProperties);
    }

    static void TearDownTestCase()
    {
        delete s_rasterizerTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

    void verifyTextureActive(TextureMaps textureMap);
};

VertexData RasterizerTest::s_vertexProperties;
::testing::StrictMock<RasterizerTestable>* RasterizerTest::s_rasterizerTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(RasterizerTest, CreateRasterizerProperlySetsMemberVariables)
{
    verifyCreateRasterizerProperlySetsMemberVariables(*s_rasterizerTestable, s_vertexProperties);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerTest, CreateVAOCorrectlyCreatesVAOWithCorrectAttributes)
{
    // FUT
    unsigned int newVAO = s_rasterizerTestable->createNewVAO();
    verifyCreateVAOPropertlySetsVertexAttributes(*s_rasterizerTestable, s_vertexProperties, newVAO, s_vertexProperties.attributesPerVertex);
}

// ------------------------------------------------------------------------
void RasterizerTest::verifyTextureActive(TextureMaps textureMap)
{
    if (textureMap.diffuseMap != RasterizerTestable::INVALID_TEXTURE_ID)
        EXPECT_CALL(*s_rasterizerTestable, activateTexture(RasterizerTestable::VALID_DIFFUSE_TEXTURE_ID)).Times(1);
    if (textureMap.specularMap != RasterizerTestable::INVALID_TEXTURE_ID)
        EXPECT_CALL(*s_rasterizerTestable, activateTexture(RasterizerTestable::VALID_SPECULAR_TEXTURE_ID)).Times(1);

    s_rasterizerTestable->activateTextures(textureMap); // FUT
}

// ------------------------------------------------------------------------
TEST_F(RasterizerTest, ActivateTextureCorrectlyCallsActivationBasedOnDiffuseAndSpecularDefinition)
{
    verifyTextureActive({ RasterizerTestable::VALID_DIFFUSE_TEXTURE_ID, RasterizerTestable::VALID_SPECULAR_TEXTURE_ID });
    verifyTextureActive({ RasterizerTestable::VALID_DIFFUSE_TEXTURE_ID, RasterizerTestable::INVALID_TEXTURE_ID });
    verifyTextureActive({ RasterizerTestable::INVALID_TEXTURE_ID, RasterizerTestable::VALID_SPECULAR_TEXTURE_ID });
    verifyTextureActive({ RasterizerTestable::INVALID_TEXTURE_ID, RasterizerTestable::INVALID_TEXTURE_ID });
}


// ===============================================================
// InstancedRasterizerTestable
// ===============================================================
class InstancedRasterizerTestable : public InstancedRasterizer {
public:
    InstancedRasterizerTestable(VertexData vertexProperties, std::vector<std::array<float, MathUtils::MAT4_SIZE>> modelMatrices, std::vector<float> textureLayerIDs)
        : InstancedRasterizer(vertexProperties, modelMatrices, textureLayerIDs) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    unsigned int getAttribVBO() { return m_attribVBO; }
    std::vector<unsigned int> getVAOs() { return m_VAOs; }
    unsigned int getInstancedModelMatrixVBO() { return m_instancedModelMatrixVBO; }
    unsigned int getInstancedTextureVBO() { return m_instancedTextureVBO; }

};

// ===============================================================
// InstancedRasterizerTest
// ===============================================================
class InstancedRasterizerTest : public RasterizerTest
{
public:
    static std::vector<std::array<float, MathUtils::MAT4_SIZE>> s_modelMatrices;
    static std::vector<float> s_textureIDs;
    static ::testing::StrictMock<InstancedRasterizerTestable>* s_instancedRasterizerTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_modelMatrices.insert(s_modelMatrices.end(), { IDENTITY_MATRIX, IDENTITY_MATRIX, IDENTITY_MATRIX });
        s_textureIDs.insert(s_textureIDs.end(), { 0.0f, 1.0f, 2.0f });
        s_instancedRasterizerTestable = new ::testing::StrictMock<InstancedRasterizerTestable>(RasterizerTest::s_vertexProperties, s_modelMatrices, s_textureIDs);
    }

    static void TearDownTestCase()
    {
        delete s_instancedRasterizerTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

};

std::vector<std::array<float, MathUtils::MAT4_SIZE>> InstancedRasterizerTest::s_modelMatrices;
std::vector<float> InstancedRasterizerTest::s_textureIDs;
::testing::StrictMock<InstancedRasterizerTestable>* InstancedRasterizerTest::s_instancedRasterizerTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(InstancedRasterizerTest, CreateInstancedRasterizerProperlySetsMemberVariables)
{
    // FUT = constructor
    verifyCreateRasterizerProperlySetsMemberVariables(*s_instancedRasterizerTestable, s_vertexProperties);

    // Model matrix and texture VBO IDs are non zero and are proper size
    GLint bufferSize;
    EXPECT_TRUE(s_instancedRasterizerTestable->getInstancedModelMatrixVBO() > 0);
    glBindBuffer(GL_ARRAY_BUFFER, s_instancedRasterizerTestable->getInstancedModelMatrixVBO());
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    EXPECT_EQ(bufferSize, sizeof(float) * s_modelMatrices.size() * MathUtils::MAT4_SIZE);
     
    EXPECT_TRUE(s_instancedRasterizerTestable->getInstancedTextureVBO() > 0);
    glBindBuffer(GL_ARRAY_BUFFER, s_instancedRasterizerTestable->getInstancedTextureVBO());
    glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    EXPECT_EQ(bufferSize, sizeof(float) * s_textureIDs.size());
}

// ------------------------------------------------------------------------
TEST_F(InstancedRasterizerTest, CreateVAOProperlySetsInstancedAttributesAndSuccessfullyDrawsArrays)
{
    // FUT
    unsigned int newVAO = s_instancedRasterizerTestable->InstancedRasterizer::createNewVAO();

    // Verify inherited params set
    verifyCreateVAOPropertlySetsVertexAttributes(*s_instancedRasterizerTestable, s_vertexProperties, newVAO, RasterizerTest::s_vertexProperties.attributesPerVertex + MathUtils::VEC4_SIZE + 1);

    // Confirm VAO instanced attributes
    GLint numVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttribs);

    // Iterate over each vertex attribute
    for (GLint attribIndex = 0; attribIndex < numVertexAttribs; ++attribIndex)
    {
        bool attribCheckCondition = attribIndex >= static_cast<GLint>(RasterizerTest::s_vertexProperties.attributesPerVertex) && attribIndex < static_cast<GLint>(RasterizerTest::s_vertexProperties.attributesPerVertex + MathUtils::VEC4_SIZE) + 1;
        GLint expectedSize = (attribIndex == RasterizerTest::s_vertexProperties.attributesPerVertex + MathUtils::VEC4_SIZE) ? 1 : 4;
        GLint expectedStride = (attribIndex == RasterizerTest::s_vertexProperties.attributesPerVertex + MathUtils::VEC4_SIZE) ? sizeof(float) : MathUtils::MAT4_SIZE * sizeof(float);
        verifyVertexAttributes(attribIndex, attribCheckCondition, expectedSize, GL_FLOAT, expectedStride, 1);
    }

    // Check that no error is output after draw arrays call
    s_instancedRasterizerTestable->drawArrays(newVAO, static_cast<unsigned int>(s_textureIDs.size()));

    GLenum error = glGetError();
    EXPECT_EQ(GL_NO_ERROR, error);
}


// ===============================================================
// NonInstancedRasterizerTestable
// ===============================================================
class NonInstancedRasterizerTestable : public NonInstancedRasterizer 
{
public:
    NonInstancedRasterizerTestable(VertexData vertexProperties)
        : NonInstancedRasterizer(vertexProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    unsigned int getAttribVBO() { return m_attribVBO; }
    std::vector<unsigned int> getVAOs() { return m_VAOs; }

};

// ===============================================================
// NonInstancedRasterizerTest
// ===============================================================
class NonInstancedRasterizerTest : public RasterizerTest
{
public:
    static ::testing::StrictMock<NonInstancedRasterizerTestable>* s_nonInstancedRasterizerTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_nonInstancedRasterizerTestable = new ::testing::StrictMock<NonInstancedRasterizerTestable>(RasterizerTest::s_vertexProperties);
    }

    static void TearDownTestCase()
    {
        delete s_nonInstancedRasterizerTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

};

::testing::StrictMock<NonInstancedRasterizerTestable>* NonInstancedRasterizerTest::s_nonInstancedRasterizerTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(NonInstancedRasterizerTest, CreateNonInstancedRasterizerProperlySetsMemberVariables)
{
    // FUT = constructor
    verifyCreateRasterizerProperlySetsMemberVariables(*s_nonInstancedRasterizerTestable, s_vertexProperties);
}

// ------------------------------------------------------------------------
TEST_F(NonInstancedRasterizerTest, CreateNewNonInstancedVAOCallsInheritedFunctionAndSuccessfullyDrawsArrays)
{
    // FUT
    unsigned int newVAO = s_nonInstancedRasterizerTestable->createNewVAO();
    verifyCreateVAOPropertlySetsVertexAttributes(*s_nonInstancedRasterizerTestable, s_vertexProperties, newVAO, s_vertexProperties.attributesPerVertex);
}



// ===============================================================
// SkyboxRasterizerTestable
// ===============================================================
class SkyboxRasterizerTestable : public SkyboxRasterizer
{
public:
    SkyboxRasterizerTestable(VertexData vertexProperties)
        : SkyboxRasterizer(vertexProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    unsigned int getAttribVBO() { return m_attribVBO; }
    std::vector<unsigned int> getVAOs() { return m_VAOs; }

};

// ===============================================================
// SkyboxRasterizerTest
// ===============================================================
class SkyboxRasterizerTest : public RasterizerTest
{
public:
    static ::testing::StrictMock<SkyboxRasterizerTestable>* s_skyboxRasterizerTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_skyboxRasterizerTestable = new ::testing::StrictMock<SkyboxRasterizerTestable>(RasterizerTest::s_vertexProperties);
    }

    static void TearDownTestCase()
    {
        delete s_skyboxRasterizerTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

};

::testing::StrictMock<SkyboxRasterizerTestable>* SkyboxRasterizerTest::s_skyboxRasterizerTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(SkyboxRasterizerTest, CreateSkyboxRasterizerProperlySetsMemberVariables)
{
    // FUT = constructor
    verifyCreateRasterizerProperlySetsMemberVariables(*s_skyboxRasterizerTestable, s_vertexProperties);
}

// ------------------------------------------------------------------------
TEST_F(SkyboxRasterizerTest, CreateSkyboxVAOProperlySetsAttributesAndEnablesSuccessfullyDrawing)
{
    // FUT
    unsigned int newVAO = s_skyboxRasterizerTestable->createNewVAO();
    
    // Confirm Skybox VAO 
    GLint numVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttribs);

    // Iterate over each vertex attribute
    constexpr int NUM_SKYBOX_ATTRIBUTES = 1;
    for (GLint attribIndex = 0; attribIndex < numVertexAttribs; ++attribIndex)
    {
        bool attribCheckCondition = attribIndex < NUM_SKYBOX_ATTRIBUTES;
        verifyVertexAttributes(attribIndex, attribCheckCondition, 3, GL_FLOAT, 3 * sizeof(float), 0);
    }

    // FUT
    s_skyboxRasterizerTestable->drawArrays(newVAO);

    GLenum error = glGetError();
    EXPECT_EQ(GL_NO_ERROR, error);
}


// ===============================================================
// TextRasterizerTestable
// ===============================================================
class TextRasterizerTestable : public TextRasterizer
{
public:
    TextRasterizerTestable(VertexData vertexProperties)
        : TextRasterizer(vertexProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    unsigned int getAttribVBO() { return m_attribVBO; }
    std::vector<unsigned int> getVAOs() { return m_VAOs; }
    unsigned int getVAO() { return m_VAO; }
    unsigned int getVBO() { return m_VBO; }
    std::map<GLchar, Character> getCharacterMap() { return m_characters; }

    // ===============================================================
    // Mock Methods
    // ===============================================================
    MOCK_METHOD(float, renderCharacter, (const char c, const TextProperties& textProperties, const float originalXPos), (override));

};

// ===============================================================
// TextRasterizerTest
// ===============================================================
class TextRasterizerTest : public RasterizerTest
{
public:
    static ::testing::StrictMock<TextRasterizerTestable>* s_textRasterizerTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_textRasterizerTestable = new ::testing::StrictMock<TextRasterizerTestable>(RasterizerTest::s_vertexProperties);
    }

    static void TearDownTestCase()
    {
        delete s_textRasterizerTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

};

::testing::StrictMock<TextRasterizerTestable>* TextRasterizerTest::s_textRasterizerTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(TextRasterizerTest, CreateTextRasterizerProperlySetsMemberVariables)
{
    // FUT = constructor
    verifyCreateRasterizerProperlySetsMemberVariables(*s_textRasterizerTestable, s_vertexProperties);
}

// ------------------------------------------------------------------------
TEST_F(TextRasterizerTest, CreateNewTextVAOCallsInheritedFunctionAndSuccessfullyDrawsArrays)
{
    // FUT
    std::shared_ptr<ShaderMock> shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");

    EXPECT_CALL(*shaderMock, use()).Times(1);
    EXPECT_CALL(*shaderMock, setMat4(::testing::StrEq("projection"), ::testing::_)).Times(1);

    unsigned int newVAO = s_textRasterizerTestable->createNewVAO(shaderMock);

    // Check that characters were set correctly
    std::map<GLchar, Character> characterMap = s_textRasterizerTestable->getCharacterMap();
    for (unsigned char c = 0; c < 128; c++)
    {
        auto iter = characterMap.find(c);
        if (iter != characterMap.end())
        {
            Character character = iter->second;
            EXPECT_TRUE(iter->second.TextureID > 0);
            EXPECT_TRUE(iter->second.Advance > 0);
        }
        else
        {
            EXPECT_TRUE(false) << "all first 128 ASCII characters should be found";
        }
    }

    // Bind VBO and VAO
    glBindBuffer(GL_ARRAY_BUFFER, s_textRasterizerTestable->getVBO());
    glBindVertexArray(newVAO);

    // Verify Text VBO
    GLint bufferSize; glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    EXPECT_EQ(bufferSize, 6 * 4 * sizeof(float));
    
    // Confirm Text VAO 
    GLint numVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttribs);

    // Iterate over each vertex attribute
    constexpr int NUM_TEXT_ATTRIBUTES = 1;
    for (GLint attribIndex = 0; attribIndex < numVertexAttribs; ++attribIndex)
    {
        bool attribCheckCondition = attribIndex < NUM_TEXT_ATTRIBUTES;
        verifyVertexAttributes(attribIndex, attribCheckCondition, 4, GL_FLOAT, 4 * sizeof(float), 0);
    }
}

// ------------------------------------------------------------------------
TEST_F(TextRasterizerTest, DrawArraysRendersEveryCharacterInInputString)
{
    // FUT
    std::shared_ptr<ShaderMock> shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");

    EXPECT_CALL(*shaderMock, use()).Times(1);
    EXPECT_CALL(*shaderMock, setMat4(::testing::StrEq("projection"), ::testing::_)).Times(1);

    s_textRasterizerTestable->createNewVAO(shaderMock);

    TextProperties textProperties = { "Test", { 50.0f, 50.0f }, 2.0f, { 1.0f, 1.0f, 1.0f } };
    
    EXPECT_CALL(*s_textRasterizerTestable, renderCharacter(::testing::Eq('T'), ::testing::Eq(textProperties), ::testing::_)).Times(1);
    EXPECT_CALL(*s_textRasterizerTestable, renderCharacter(::testing::Eq('e'), ::testing::Eq(textProperties), ::testing::_)).Times(1);
    EXPECT_CALL(*s_textRasterizerTestable, renderCharacter(::testing::Eq('s'), ::testing::Eq(textProperties), ::testing::_)).Times(1);
    EXPECT_CALL(*s_textRasterizerTestable, renderCharacter(::testing::Eq('t'), ::testing::Eq(textProperties), ::testing::_)).Times(1);

    s_textRasterizerTestable->drawArrays(textProperties);

    GLenum error = glGetError();
    EXPECT_EQ(GL_NO_ERROR, error);
}

// ===============================================================
// RasterizerUtilityTestable
// ===============================================================
class RasterizerUtilityTestable : public RasterizerUtility
{
public:
    RasterizerUtilityTestable()
        : RasterizerUtility() {}
};

// ===============================================================
// RasterizerUtilityTest
// ===============================================================
class RasterizerUtilityTest : public OpenGLTestFixture
{
};

// ------------------------------------------------------------------------
TEST_F(RasterizerUtilityTest, SetDepthEnablesAndDisablesDepthCorrectly)
{
    RasterizerUtilityTestable::setDepthTestState(false); // FUT
    bool isDepthEnabled = glIsEnabled(GL_DEPTH_TEST); EXPECT_EQ(false, isDepthEnabled);
    
    RasterizerUtilityTestable::setDepthTestState(true); // FUT
    isDepthEnabled = glIsEnabled(GL_DEPTH_TEST); EXPECT_EQ(true, isDepthEnabled);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerUtilityTest, SetDepthFuncCorrectlySetsBasedOnInput)
{
    RasterizerUtilityTestable::setDepthMethod(DepthMethods::DEPTH_LEQUAL); // FUT
    GLenum depthFunc; glGetIntegerv(GL_DEPTH_FUNC, reinterpret_cast<GLint*>(&depthFunc)); EXPECT_EQ(GL_LEQUAL, depthFunc);

    RasterizerUtilityTestable::setDepthMethod(DepthMethods::DEPTH_LESS); // FUT
    depthFunc; glGetIntegerv(GL_DEPTH_FUNC, reinterpret_cast<GLint*>(&depthFunc)); EXPECT_EQ(GL_LESS, depthFunc);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerUtilityTest, SetBlendStateCorrectlyEnablesDisablesBlendState)
{
    RasterizerUtilityTestable::setBlendState(true); // FUT
    bool isBlendEnabled = glIsEnabled(GL_BLEND); EXPECT_EQ(true, isBlendEnabled);
    GLenum blendSrcFactor; glGetIntegerv(GL_BLEND_DST_ALPHA, reinterpret_cast<GLint*>(&blendSrcFactor)); EXPECT_EQ(blendSrcFactor, GL_ONE_MINUS_SRC_ALPHA);

    RasterizerUtilityTestable::setBlendState(false); // FUT
    isBlendEnabled = glIsEnabled(GL_BLEND); EXPECT_EQ(false, isBlendEnabled);
    blendSrcFactor; glGetIntegerv(GL_BLEND_DST_ALPHA, reinterpret_cast<GLint*>(&blendSrcFactor)); EXPECT_EQ(blendSrcFactor, GL_ONE_MINUS_SRC_ALPHA);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerUtilityTest, SetSuperSampleStateCorrectlyEnablesDisablesSuperSampleState)
{
    RasterizerUtilityTestable::setSupersampleState(true); // FUT
    bool isSupersampleEnabled = glIsEnabled(GL_MULTISAMPLE); EXPECT_EQ(true, isSupersampleEnabled);

    RasterizerUtilityTestable::setSupersampleState(false); // FUT
    isSupersampleEnabled = glIsEnabled(GL_MULTISAMPLE); EXPECT_EQ(false, isSupersampleEnabled);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerUtilityTest, SetFaceCullingCorrectlyEnablesDisablesFaceCulling)
{
    RasterizerUtilityTestable::enableFaceCulling(); // FUT
    bool isSupersampleEnabled = glIsEnabled(GL_CULL_FACE); EXPECT_EQ(true, isSupersampleEnabled);
    GLenum currentCullFaceMode; glGetIntegerv(GL_CULL_FACE_MODE, reinterpret_cast<GLint*>(&currentCullFaceMode)); EXPECT_EQ(GL_BACK, currentCullFaceMode);
    GLenum currentFrontFaceOrientation; glGetIntegerv(GL_FRONT_FACE, reinterpret_cast<GLint*>(&currentFrontFaceOrientation)); EXPECT_EQ(GL_CCW, currentFrontFaceOrientation);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerUtilityTest, SetWireframeCorrectlySetsPolygonMode)
{
    RasterizerUtilityTestable::setWireframeDrawingState(true);
    GLenum currentFrontMode, currentBackMode;
    glGetIntegerv(GL_POLYGON_MODE, reinterpret_cast<GLint*>(&currentFrontMode));
    glGetIntegerv(GL_POLYGON_MODE, reinterpret_cast<GLint*>(&currentBackMode));
    EXPECT_EQ(GL_LINE, currentFrontMode); EXPECT_EQ(GL_LINE, currentBackMode);

    RasterizerUtilityTestable::setWireframeDrawingState(false);
    glGetIntegerv(GL_POLYGON_MODE, reinterpret_cast<GLint*>(&currentFrontMode));
    glGetIntegerv(GL_POLYGON_MODE, reinterpret_cast<GLint*>(&currentBackMode));
    EXPECT_EQ(GL_FILL, currentFrontMode); EXPECT_EQ(GL_FILL, currentBackMode);
}
