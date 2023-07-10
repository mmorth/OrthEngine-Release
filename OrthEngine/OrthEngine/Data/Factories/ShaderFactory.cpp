#include "ShaderFactory.hpp"

// ------------------------------------------------------------------------
ShaderFactory& ShaderFactory::getInstance()
{
	static ShaderFactory instance;
	return instance;
}

// ------------------------------------------------------------------------
ShaderFactory::ShaderFactory()
{
	// ===== Object Shaders =====
	m_shaderMap["Default_Object"] = std::make_shared<Shader>("object_shader.vert", "object_shader.frag");
	m_shaderMap["Default_InstancedObject"] = std::make_shared<Shader>("instanced_shader.vert", "instanced_shader.frag");
	m_shaderMap["Default_Light"] = std::make_shared<Shader>("light_shader.vert", "light_shader.frag");
	m_shaderMap["Default_Skybox"] = std::make_shared<Shader>("skybox_shader.vert", "skybox_shader.frag");
	m_shaderMap["Default_Text"] = std::make_shared<Shader>("text_shader.vert", "text_shader.frag");

	// ===== Buffer Shaders =====
	m_shaderMap["Default_Framebuffer"] = std::make_shared<Shader>("framebuffer_shader.vert", "framebuffer_shader.frag");
	m_shaderMap["Default_Outline"] = std::make_shared<Shader>("outline_shader.vert", "outline_shader.frag");

	// ===== Normal Shaders =====
	m_shaderMap["Default_Normal"] = std::make_shared<Shader>("normal_shader.vert", "normal_shader.frag", "normal_shader.geom");
	m_shaderMap["Default_InstancedNormal"] = std::make_shared<Shader>("instanced_normal_shader.vert", "normal_shader.frag", "normal_shader.geom");

	LOG(INFO) << "Default shaders created";
}

// ------------------------------------------------------------------------
void ShaderFactory::addShader(const std::string& key, const ShaderFileNames& shader)
{
	m_shaderMap.emplace(key, std::make_shared<Shader>(shader.fragmentShader.c_str(), shader.fragmentShader.c_str(), shader.geometryShader.c_str()));

	LOG(INFO) << "Shader added: " << key;
}

// ------------------------------------------------------------------------
std::optional<std::shared_ptr<Shader>> ShaderFactory::getShader(const std::string& key)
{
	auto it = m_shaderMap.find(key);
	LOG(INFO) << "Shader retrieved " << key;

	if (it != m_shaderMap.end())
		return it->second;
	else
		return std::nullopt;
}
