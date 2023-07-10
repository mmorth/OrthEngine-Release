#include "Framebuffer.hpp"

// ------------------------------------------------------------------------
Framebuffer::Framebuffer(const std::shared_ptr<Shader> shader, const VertexData& vertexData)
    : m_shader(shader)
    , m_vertexData(vertexData)
{
    LOG(INFO) << "ctor";
    
    // Obtain the window width and height from GlobalSettings
    unsigned int windowWidth = GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200);
    unsigned int windowHeight = GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800);

    // generate quad VAO for screen rendering
    generateQuadVao();

    // generate frame and render buffer for MSAA
    generateFramebuffer(windowWidth, windowHeight, false);
    generateRenderbuffer(windowWidth, windowHeight);

    // check if the framebuffer is complete
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Renderbuffer is not complete. Attach texture or render buffer");
    bindFramebuffer(FramebufferTypes::DEFAULT);

    // generate frame buffer for post-processing
    generateFramebuffer(windowWidth, windowHeight, true);

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        throw std::runtime_error("Renderbuffer is not complete. Attach texture or render buffer");
    bindFramebuffer(FramebufferTypes::DEFAULT);
}

// ------------------------------------------------------------------------
Framebuffer::~Framebuffer()
{
    glDeleteRenderbuffers(1, &m_rbo);
    glDeleteFramebuffers(1, &m_framebuffer);
    glDeleteVertexArrays(1, &m_quadVAO);
    glDeleteBuffers(1, &m_quadVBO);

    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void Framebuffer::generateFramebuffer(const unsigned int screenWidth, const unsigned int screenHeight, const bool isMSAA)
{
    if (isMSAA)
    {
        // create MSAA framebuffer
        glGenFramebuffers(1, &m_framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

        // create a multisampled color attachment texture
        glGenTextures(1, &m_textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorbuffer);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, GlobalSettings::getInstance().getValue<unsigned int>("numSamples", 4), GL_RGB, screenWidth, screenHeight, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_textureColorbuffer, 0);

        LOG(INFO) << "MSAA Framebuffer generated " << m_framebuffer;
    }
    else
    {
        // create post-processing framebuffer
        glGenFramebuffers(1, &m_intermediateFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, m_intermediateFBO);

        // create a color attachment texture
        glGenTextures(1, &m_screenTexture);
        glBindTexture(GL_TEXTURE_2D, m_screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);

        LOG(INFO) << "Non-MSAA Framebuffer generated " << m_intermediateFBO;
    }
}

// ------------------------------------------------------------------------
void Framebuffer::generateRenderbuffer(const unsigned int screenWidth, const unsigned int screenHeight)
{
    // create render buffer for depth and stencil buffers
    glGenRenderbuffers(1, &m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, GlobalSettings::getInstance().getValue<unsigned int>("numSamples", 4), GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

    LOG(INFO) << "Renderbuffer generated " << m_rbo;
}

// ------------------------------------------------------------------------
void Framebuffer::generateQuadVao()
{
    // generate quad VAO for post-process rendering
    glGenVertexArrays(1, &m_quadVAO);
    glGenBuffers(1, &m_quadVBO);
    glBindVertexArray(m_quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_quadVBO);

    glBufferData(GL_ARRAY_BUFFER, m_vertexData.vertexAttributes.size() * sizeof(float), m_vertexData.vertexAttributes.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

    LOG(INFO) << "Quad VAO created " << m_quadVAO;
}

// ------------------------------------------------------------------------
void Framebuffer::updateFramebufferSize(const unsigned int screenWidth, const unsigned int screenHeight, const bool isMSAA)
{
    // Bind framebuffer to apply changes to
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    if (isMSAA)
    {
        // Update MSAA screen size
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, m_textureColorbuffer);
        glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, GlobalSettings::getInstance().getValue<unsigned int>("numSamples", 4), GL_RGB, screenWidth, screenHeight, GL_TRUE);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, m_textureColorbuffer, 0);
        glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    }
    else
    {
        // Update screen size
        glBindTexture(GL_TEXTURE_2D, m_screenTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, screenWidth, screenHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_screenTexture, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    LOG(INFO) << "Framebuffer size updated to (Width, Height) = (" << screenWidth << ", " << screenHeight << ")";
}

// ------------------------------------------------------------------------
void Framebuffer::updateRenderbufferSize(const unsigned int screenWidth, const unsigned int screenHeight)
{
    // Bind framebuffer and render buffer to apply screen size changes to
    glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

    glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, GlobalSettings::getInstance().getValue<unsigned int>("numSamples", 4), GL_DEPTH24_STENCIL8, screenWidth, screenHeight);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    LOG(INFO) << "Renderbuffer size updated to (Width, Height) = (" << screenWidth << ", " << screenHeight << ")";
}

// ------------------------------------------------------------------------
void Framebuffer::bindFramebuffer(const FramebufferTypes framebufferType)
{
    glBindVertexArray(0);
    
    unsigned int framebufferID = 0;
    switch (framebufferType)
    {
        case FramebufferTypes::DEFAULT:
            framebufferID = 0;
            LOG(INFO) << "Bound default framebuffer";
            break;
        case FramebufferTypes::MSAA:
            framebufferID = m_framebuffer;
            LOG(INFO) << "Bound MSAA framebuffer";
            break;
        case FramebufferTypes::POST:
            framebufferID = m_intermediateFBO;
            LOG(INFO) << "Bound post framebuffer";
            break;
        default:
            break;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, framebufferID);
}

// ------------------------------------------------------------------------
void Framebuffer::bindAndDrawQuadVAO()
{
    m_shader->use();
    m_shader->setInt("screenTexture", m_screenTexture);
    glActiveTexture(GL_TEXTURE0 + m_screenTexture);
    glBindTexture(GL_TEXTURE_2D, m_screenTexture);
    glBindVertexArray(m_quadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    LOG(INFO) << "Bind and draw Framebuffer quad";
}

// ------------------------------------------------------------------------
void Framebuffer::blitMSAA()
{
    unsigned int screenWidth = GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200);
    unsigned int screenHeight = GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800);
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_framebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_intermediateFBO);
    glBlitFramebuffer(0, 0, screenWidth, screenHeight, 0, 0, screenWidth, screenHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);

    LOG(INFO) << "Framebuffer blitMSAA";
}

// ------------------------------------------------------------------------
std::vector<unsigned char> Framebuffer::getFramebufferContents()
{
    // Store framebuffer render contents
    // Obtain the window width and height from GlobalSettings
    unsigned int framebufferWidth = GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200);
    unsigned int framebufferHeight = GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800);

    std::vector<unsigned char> pixels(static_cast<size_t>(framebufferWidth) * static_cast<size_t>(framebufferHeight) * 4); // RGBA format
    glReadPixels(0, 0, framebufferWidth, framebufferHeight, GL_RGBA, GL_UNSIGNED_BYTE, pixels.data());

    return pixels;
}
