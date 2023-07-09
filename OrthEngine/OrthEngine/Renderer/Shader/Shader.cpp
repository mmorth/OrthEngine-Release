#include "Shader.hpp"

// ------------------------------------------------------------------------
Shader::Shader(const char* vertexPath, const char* fragmentPath, const char* geometryPath)
    : m_isTestRun(false)
    , m_ID(0)
{
    //LOG(INFO) << "ctor";

    // Read the vertex and fragment code from a file
    std::string vertexCodeString = readShaderFromFile(vertexPath);
    std::string fragmentCodeString = readShaderFromFile(fragmentPath);
    const char* vertexCode = vertexCodeString.c_str();
    const char* fragmentCode = fragmentCodeString.c_str();
    
    // Compile the shaders and attach them to the program shader
    unsigned int vertexShaderID = compileShader(ShaderTypes::VERTEX, vertexCode);
    unsigned int fragmentShaderID = compileShader(ShaderTypes::FRAGMENT, fragmentCode);

    unsigned int geometryShaderID = 0;
    if (geometryPath != nullptr)
    {
        std::string geometryCodeString = readShaderFromFile(geometryPath);
        const char* geometryCode = geometryCodeString.c_str();
        geometryShaderID = compileShader(ShaderTypes::GEOMETRY, geometryCode);
    }

    compileProgramShader(vertexShaderID, fragmentShaderID, geometryShaderID);
    deleteShaders(vertexShaderID, fragmentShaderID);

    //LOG(INFO) << "New shader ID " << m_ID << " created";

    // Use the shader
    use();
}

// ------------------------------------------------------------------------
Shader::~Shader() 
{
    //LOG(INFO) << "dtor";
    
    if (!m_isTestRun)
        glDeleteProgram(m_ID);
}

// ------------------------------------------------------------------------
void Shader::deleteShaders(const unsigned int vertexShaderID, const unsigned int fragmentShaderID)
{
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);
}

// ------------------------------------------------------------------------
void Shader::use() const
{
    glUseProgram(m_ID);
}

// ------------------------------------------------------------------------
unsigned int Shader::getID() const
{
    return m_ID;
}

// ------------------------------------------------------------------------
unsigned int Shader::compileShader(const ShaderTypes shaderType, const char* shaderCode)
{
    unsigned int glShaderType = 0;
    std::string shaderString = "";

    switch (shaderType)
    {
        case ShaderTypes::VERTEX:
            glShaderType = GL_VERTEX_SHADER;
            shaderString = "VERTEX";
            break;
        case ShaderTypes::GEOMETRY:
            glShaderType = GL_GEOMETRY_SHADER;
            shaderString = "GEOMETRY";
            break;
        case ShaderTypes::FRAGMENT:
            glShaderType = GL_FRAGMENT_SHADER;
            shaderString = "FRAGMENT";
            break;
        default:
            break;
    }

    unsigned int shaderID = glCreateShader(glShaderType);
    glShaderSource(shaderID, 1, &shaderCode, NULL);
    glCompileShader(shaderID);
    checkCompileErrors(shaderID, shaderString);

    return shaderID;
}

// ------------------------------------------------------------------------
void Shader::compileProgramShader(const unsigned int vertexShaderID, const unsigned int fragmentShaderID, const unsigned int geometryShaderID)
{
    m_ID = glCreateProgram();
    glAttachShader(m_ID, vertexShaderID);
    glAttachShader(m_ID, fragmentShaderID);

    if (geometryShaderID != 0)
        glAttachShader(m_ID, geometryShaderID);

    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, "PROGRAM");
}

// ------------------------------------------------------------------------
void Shader::checkCompileErrors(const GLuint shader, const std::string& type)
{
    GLint success;
    GLchar infoLog[1024];
    if (type == "PROGRAM")
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog); 
            throw std::runtime_error("ERROR::PROGRAM_LINKING_ERROR of type: " + type + "\n" + infoLog);
        }
    }
    else
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
        }
    }
}

// ------------------------------------------------------------------------
std::string Shader::readShaderFromFile(const char* filepath)
{
    std::ifstream fileStream;
    fileStream.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        fileStream.open(filepath);
        std::stringstream shaderSourceStream;
        shaderSourceStream << fileStream.rdbuf();
        fileStream.close();
        return shaderSourceStream.str();
    }
    catch (const std::ifstream::failure& e)
    {
        throw std::runtime_error("Failed to read shader file " + std::string(filepath) + ". Error: " + e.what());
    }
}

// ------------------------------------------------------------------------
void Shader::setBool(const std::string& name, const bool value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setInt(const std::string& name, const int value) const
{
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setFloat(const std::string& name, const float value) const
{
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const std::string& name, const float* value) const
{
    glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, value);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const std::string& name, const float* value) const
{
    glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, value);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const std::string& name, const float* value) const
{
    glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, value);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const std::string& name, const float* value) const
{
    glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const std::string& name, const float* value) const
{
    glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const std::string& name, const float* value) const
{
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, value);
}
