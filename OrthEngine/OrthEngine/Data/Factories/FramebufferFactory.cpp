#include "FramebufferFactory.hpp"

// ------------------------------------------------------------------------
FramebufferFactory& FramebufferFactory::getInstance()
{
	static FramebufferFactory instance;
	return instance;
}

// ------------------------------------------------------------------------
FramebufferFactory::FramebufferFactory()
	: m_shaderFactory(std::make_shared<ShaderFactory>(ShaderFactory::getInstance()))
	, m_vertexDataFactory(std::make_shared<VertexDataFactory>(VertexDataFactory::getInstance()))
{
	//LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
void FramebufferFactory::initialize(const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<VertexDataFactory> vertexDataFactory)
{
	m_shaderFactory = shaderFactory;
	m_vertexDataFactory = vertexDataFactory;

	//LOG(INFO) << "FramebufferFactory mocks set";
}

// ------------------------------------------------------------------------
std::unique_ptr<Framebuffer> FramebufferFactory::getFramebuffer()
{
	//LOG(INFO) << "Framebuffer retrieved";
	return std::make_unique<Framebuffer>(m_shaderFactory->getShader("Default_Framebuffer").value(), m_vertexDataFactory->getVertexData("Default_Quad").value());
}
