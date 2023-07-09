#ifndef SHADERMOCK_HPP
#define SHADERMOCK_HPP

#include <gmock/gmock.h>

#include "Shader/Shader.hpp"

class ShaderMock : public Shader
{
public:
    ShaderMock(const char* vertexPath, const char* fragmentPath, const char* geometryPath = nullptr) { m_isTestRun = true; }
    ~ShaderMock() {};

    MOCK_METHOD(void, use, (), (const override));
    MOCK_METHOD(unsigned int, getID, (), (const override));
    MOCK_METHOD(void, setBool, (const std::string& name, const bool value), (const override));
    MOCK_METHOD(void, setInt, (const std::string& name, const int value), (const override));
    MOCK_METHOD(void, setFloat, (const std::string& name, const float value), (const override));
    MOCK_METHOD(void, setVec2, (const std::string& name, const float* value), (const override));
    MOCK_METHOD(void, setVec3, (const std::string& name, const float* value), (const override));
    MOCK_METHOD(void, setVec4, (const std::string& name, const float* value), (const override));
    MOCK_METHOD(void, setMat2, (const std::string& name, const float* value), (const override));
    MOCK_METHOD(void, setMat3, (const std::string& name, const float* value), (const override));
    MOCK_METHOD(void, setMat4, (const std::string& name, const float* data), (const override));
};

#endif // SHADERMOCK_HPP
