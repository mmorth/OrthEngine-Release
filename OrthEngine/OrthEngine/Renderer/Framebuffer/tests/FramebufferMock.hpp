#ifndef FRAMEBUFFERMOCK_HPP
#define FRAMEBUFFERMOCK_HPP

#include <gmock/gmock.h>

#include "Framebuffer/Framebuffer.hpp"

class FramebufferMock : public Framebuffer 
{
public:
    FramebufferMock(std::shared_ptr<Shader> shader, VertexData vertexData)
        : Framebuffer(shader, vertexData)
    {
        // Additional constructor logic for FramebufferMock if needed
    }

    MOCK_METHOD(void, updateFramebufferSize, (const unsigned int screenWidth, const unsigned int screenHeight, bool isMSAA), (override));
    MOCK_METHOD(void, updateRenderbufferSize, (const unsigned int screenWidth, const unsigned int screenHeight), (override));
    MOCK_METHOD(void, bindFramebuffer, (FramebufferTypes framebufferType), (override));
    MOCK_METHOD(void, bindAndDrawQuadVAO, (), (override));
    MOCK_METHOD(void, blitMSAA, (), (override));
};

#endif // FRAMEBUFFERMOCK_HPP
