#ifndef FRAMEBUFFER_HPP
#define FRAMEBUFFER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "GlobalSettingsConstants/GlobalSettings.hpp"
#include "Rasterizers/DataStructs.hpp"
#include "Shader/Shader.hpp"

enum class FramebufferTypes
{
    DEFAULT=0,
    MSAA=1,
    POST=2,
    TEST=3
};

class Framebuffer 
{
public:
    Framebuffer(const std::shared_ptr<Shader> shader, const VertexData& vertexData);
    ~Framebuffer();

    virtual void updateFramebufferSize(const unsigned int screenWidth, const unsigned int screenHeight, const bool isMSAA);
    virtual void updateRenderbufferSize(const unsigned int screenWidth, const unsigned int screenHeight);

    virtual void bindFramebuffer(const FramebufferTypes framebufferType);
    virtual void bindAndDrawQuadVAO();

    virtual void blitMSAA();

    virtual std::vector<unsigned char> getFramebufferContents();

protected:
    std::shared_ptr<Shader> m_shader;

    VertexData m_vertexData;

    unsigned int m_framebuffer;
    unsigned int m_intermediateFBO;
    unsigned int m_rbo;
    unsigned int m_quadVAO;
    unsigned int m_quadVBO;
    unsigned int m_textureColorbuffer;
    unsigned int m_screenTexture;

private:
    void generateFramebuffer(const unsigned int screenWidth, const unsigned int screenHeight, const bool isMSAA);
    void generateRenderbuffer(const unsigned int screenWidth, const unsigned int screenHeight);
    void generateQuadVao();

};

#endif // FRAMEBUFFER_HPP
