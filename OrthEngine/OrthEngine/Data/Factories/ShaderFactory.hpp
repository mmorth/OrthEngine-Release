#ifndef SHADERFACTORY_HPP
#define SHADERFACTORY_HPP

#include <memory>
#include <optional>
#include <unordered_map>

#include "Shader/Shader.hpp"

struct ShaderFileNames 
{
    std::string vertexShader;
    std::string fragmentShader;
    std::string geometryShader;
};

class ShaderFactory 
{
public:
    static ShaderFactory& getInstance();

    virtual void addShader(const std::string& key, const ShaderFileNames& shader);
    virtual std::optional<std::shared_ptr<Shader>> getShader(const std::string& key);

protected:
    ShaderFactory();

private:
    std::unordered_map<std::string, std::shared_ptr<Shader>> m_shaderMap;
    std::shared_ptr<Shader> m_shader;
};

#endif // SHADERFACTORY_HPP
