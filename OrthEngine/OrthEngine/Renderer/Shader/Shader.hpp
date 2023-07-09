#ifndef SHADER_HPP
#define SHADER_HPP

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <fstream>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include <filesystem>


enum class ShaderTypes {
    VERTEX=1,
    GEOMETRY=2,
    FRAGMENT=3
};

class Shader
{
public:
    Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath=nullptr);
    Shader() : m_isTestRun(false), m_ID(0) {}
    ~Shader();

    virtual void use() const;
    virtual unsigned int getID() const;
    virtual void setBool(const std::string& name, const bool value) const;
    virtual void setInt(const std::string& name, const int value) const;
    virtual void setFloat(const std::string& name, const float value) const;
    virtual void setVec2(const std::string& name, const float* value) const;
    virtual void setVec3(const std::string& name, const float* value) const;
    virtual void setVec4(const std::string& name, const float* value) const;
    virtual void setMat2(const std::string& name, const float* value) const;
    virtual void setMat3(const std::string& name, const float* value) const;
    virtual void setMat4(const std::string& name, const float* data) const;

    bool m_isTestRun;

private:
    void deleteShaders(const unsigned int vertexShaderID, const unsigned int fragmentShaderID);
    unsigned int Shader::compileShader(const ShaderTypes shaderType, const char* shaderCode);
    void compileProgramShader(const unsigned int vertexShaderID, const unsigned int fragmentShaderID, const unsigned int geometryShaderID);
    void checkCompileErrors(const GLuint shader, const std::string& type);
    static std::string readShaderFromFile(const char* filepath);

    unsigned int m_ID;
};

#endif // SHADER_HPP