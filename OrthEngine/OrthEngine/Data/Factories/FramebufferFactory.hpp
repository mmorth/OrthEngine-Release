#ifndef FRAMEBUFFERFACTORY_HPP
#define FRAMEBUFFERFACTORY_HPP

#include <memory>
#include <optional>
#include <unordered_map>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>

#include "Framebuffer/Framebuffer.hpp"
#include "ShaderFactory.hpp"
#include "VertexDataFactory.hpp"

class FramebufferFactory 
{
public:
    static FramebufferFactory& getInstance();

    virtual void initialize(const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<VertexDataFactory> vertexDataFactory);

    virtual std::unique_ptr<Framebuffer> getFramebuffer();

protected:
    FramebufferFactory();

private:
    std::shared_ptr<ShaderFactory> m_shaderFactory;
    std::shared_ptr<VertexDataFactory> m_vertexDataFactory;

};

#endif // FRAMEBUFFERFACTORY_HPP
