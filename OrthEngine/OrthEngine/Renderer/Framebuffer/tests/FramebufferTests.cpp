#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Shader/tests/ShaderMock.hpp"
#include "OpenGLFixture.hpp"
#include "Framebuffer/Framebuffer.hpp"

// ===============================================================
// FramebufferTestable
// ===============================================================
class FramebufferTestable : public Framebuffer {
public:
    FramebufferTestable(std::shared_ptr<Shader> shader, VertexData vertexData)
        : Framebuffer(shader, vertexData) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() const { return m_shader; }
    const VertexData& getVertexData() const { return m_vertexData; }
    unsigned int getFramebuffer() const { return m_framebuffer; }
    unsigned int getIntermediateFBO() const { return m_intermediateFBO; }
    unsigned int getRBO() const { return m_rbo; }
    unsigned int getQuadVAO() const { return m_quadVAO; }
    unsigned int getQuadVBO() const { return m_quadVBO; }
    unsigned int getTextureColorbuffer() const { return m_textureColorbuffer; }
    unsigned int getScreenTexture() const { return m_screenTexture; }

    // ===============================================================
    // Mock Methods
    // ===============================================================
    

    // ===============================================================
    // Constants
    // ===============================================================
    
};

// ------------------------------------------------------------------------
class FramebufferTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<ShaderMock> s_shaderMock;
    static VertexData s_vertexData;
    static ::testing::StrictMock<FramebufferTestable>* s_framebufferTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        s_vertexData = { { -1.0f,  1.0f,  0.0f, 1.0f,
                           -1.0f, -1.0f,  0.0f, 0.0f,
                            1.0f, -1.0f,  1.0f, 0.0f,

                           -1.0f,  1.0f,  0.0f, 1.0f,
                            1.0f, -1.0f,  1.0f, 0.0f,
                            1.0f,  1.0f,  1.0f, 1.0f },
            4 };
        s_framebufferTestable = new ::testing::StrictMock<FramebufferTestable>(s_shaderMock, s_vertexData);
    }

    static void TearDownTestCase()
    {
        s_shaderMock.reset();
        delete s_framebufferTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

    // ------------------------------------------------------------------------
    void verifyFramebuffer(unsigned int framebufferID, int expectedAttachment, int expectedWidth, int expectedHeight)
    {
        // bind render buffer to test
        glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);

        // verify attachment
        GLint attachment;       glGetFramebufferAttachmentParameteriv(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE, &attachment);
        EXPECT_EQ(expectedAttachment, attachment);

        // verify framebuffer width and height
        GLint width, height;    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);   glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);
        EXPECT_EQ(expectedWidth, width);    EXPECT_EQ(expectedHeight, height);
    }

    // ------------------------------------------------------------------------
    void verifyRenderbuffer(unsigned int renderbufferID, int expectedWidth, int expectedHeight, int expectedSamples)
    {
        // bind renderbuffer
        glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);

        // verify renderbuffer width and height
        GLint width, height;    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);   glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
        EXPECT_EQ(expectedWidth, width);    EXPECT_EQ(expectedHeight, height);

        // verify renderbuffer samples
        GLint samples;          glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_SAMPLES, &samples);
        EXPECT_EQ(expectedSamples, samples);
    }
};

std::shared_ptr<ShaderMock> FramebufferTest::s_shaderMock = nullptr;
VertexData FramebufferTest::s_vertexData;
::testing::StrictMock<FramebufferTestable>* FramebufferTest::s_framebufferTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(FramebufferTest, CreateFramebufferProperlySetsMemberVariables)
{
    // FUT = constructor
    EXPECT_TRUE(s_framebufferTestable->getShader() != nullptr);
    EXPECT_EQ(s_vertexData, s_framebufferTestable->getVertexData());
    EXPECT_TRUE(s_framebufferTestable->getFramebuffer() > 0);
    EXPECT_TRUE(s_framebufferTestable->getIntermediateFBO() > 0);
    EXPECT_TRUE(s_framebufferTestable->getRBO() > 0);
    EXPECT_TRUE(s_framebufferTestable->getQuadVAO() > 0);
    EXPECT_TRUE(s_framebufferTestable->getQuadVBO() > 0);
    EXPECT_TRUE(s_framebufferTestable->getTextureColorbuffer() > 0);
    EXPECT_TRUE(s_framebufferTestable->getScreenTexture() > 0);

    // Check buffer properties
    glBindBuffer(GL_ARRAY_BUFFER, s_framebufferTestable->getQuadVBO());
    GLint bufferSize; glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
    EXPECT_EQ(bufferSize, sizeof(float) * s_vertexData.vertexAttributes.size());

    // Check quad properties
    GLint numVertexAttribs;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numVertexAttribs);

    GLint numVertexAttributes = 2;
    glBindVertexArray(s_framebufferTestable->getQuadVAO());
    for (GLint attribIndex = 0; attribIndex < numVertexAttribs; ++attribIndex)
    {
        GLint enabled; glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
        if (attribIndex < numVertexAttributes)
        {
            // Obtain and verify vertex attribute data
            GLint enabled;  glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);                         EXPECT_EQ(true, enabled);
            GLint size;     glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_SIZE, &size);                               EXPECT_EQ(2, size);
            GLenum type;    glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_TYPE, reinterpret_cast<GLint*>(&type));     EXPECT_EQ(GL_FLOAT, type);
            GLint stride;   glGetVertexAttribiv(attribIndex, GL_VERTEX_ATTRIB_ARRAY_STRIDE, &stride);                           EXPECT_EQ(4 * sizeof(float), stride);
        }
        else
        {
            // All other vertex attribs should be disabled
            EXPECT_EQ(false, enabled);
        }
    }

    // Check framebuffer properties
    verifyFramebuffer(s_framebufferTestable->getFramebuffer(), GL_TEXTURE, GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200), GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800));
    verifyFramebuffer(s_framebufferTestable->getIntermediateFBO(), GL_TEXTURE, GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200), GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800));

    // Check render buffer properties
    verifyRenderbuffer(s_framebufferTestable->getRBO(), GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200), GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800), GlobalSettings::getInstance().getValue<unsigned int>("numSamples", 4));

    // Check textures are valid
    EXPECT_EQ(GL_TRUE, glIsTexture(s_framebufferTestable->getTextureColorbuffer()));
    EXPECT_EQ(GL_TRUE, glIsTexture(s_framebufferTestable->getScreenTexture()));
}

// ------------------------------------------------------------------------
TEST_F(FramebufferTest, ResizeCorrectlyResizesFramebufferAndRenderBuffer)
{
    // Verify framebuffer resize
    unsigned int newWidth = 1920;  unsigned int newHeight = 1080;
    s_framebufferTestable->updateFramebufferSize(newWidth, newHeight, true); // FUT

    glBindFramebuffer(GL_FRAMEBUFFER, s_framebufferTestable->getFramebuffer());
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, s_framebufferTestable->getTextureColorbuffer());
    GLint width, height;    glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_TEXTURE_WIDTH, &width);   glGetTexLevelParameteriv(GL_TEXTURE_2D_MULTISAMPLE, 0, GL_TEXTURE_HEIGHT, &height);
    EXPECT_EQ(newWidth, width);    EXPECT_EQ(newHeight, height);

    // Verify render buffer resize
    s_framebufferTestable->updateRenderbufferSize(newWidth, newHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, s_framebufferTestable->getRBO());
    glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);   glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);
    EXPECT_EQ(newWidth, width);    EXPECT_EQ(newHeight, height);
}

// ------------------------------------------------------------------------
TEST_F(FramebufferTest, BindFramebufferCorrectlyBindsBasedOnTypeParam)
{
    GLint currentFramebufferId;
    
    // FUT
    s_framebufferTestable->bindFramebuffer(FramebufferTypes::DEFAULT);  glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebufferId);   EXPECT_EQ(0, currentFramebufferId);
    s_framebufferTestable->bindFramebuffer(FramebufferTypes::MSAA);     glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebufferId);   EXPECT_EQ(s_framebufferTestable->getFramebuffer(), currentFramebufferId);
    s_framebufferTestable->bindFramebuffer(FramebufferTypes::POST);     glGetIntegerv(GL_FRAMEBUFFER_BINDING, &currentFramebufferId);   EXPECT_EQ(s_framebufferTestable->getIntermediateFBO(), currentFramebufferId);
}

// ------------------------------------------------------------------------
TEST_F(FramebufferTest, BindAndDrawQuadCorrectlySetsShaderTextureAndBindsAndDraw)
{
    EXPECT_CALL(*s_shaderMock, use()).Times(1);
    EXPECT_CALL(*s_shaderMock, setInt(::testing::StrEq("screenTexture"), ::testing::Eq(s_framebufferTestable->getScreenTexture()))).Times(1);

    // FUT
    s_framebufferTestable->bindAndDrawQuadVAO();

    GLenum error = glGetError();
    EXPECT_EQ(GL_NO_ERROR, error);
}

// ------------------------------------------------------------------------
TEST_F(FramebufferTest, BlitMSAACorrectlyAppliesMSAAToAllFramebuffers)
{
    // FUT
    s_framebufferTestable->blitMSAA();

    GLenum error = glGetError();
    EXPECT_EQ(GL_NO_ERROR, error);
}
