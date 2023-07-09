#include <gtest/gtest.h>

#include "OpenGLFixture.hpp"
#include "Shader/Shader.hpp"

// ===============================================================
// ShaderTestable
// ===============================================================
class ShaderTestable : public Shader {
public:
    ShaderTestable(const char* vertexPath, const char* fragmentPath, const char* geometryPath=nullptr) 
        : Shader(vertexPath, fragmentPath, geometryPath) {}
};

// ------------------------------------------------------------------------
class ShaderTest : public OpenGLTestFixture
{
protected:
    void SetUp() override {}
    void TearDown() override {}

    void verifyShaderOperations(ShaderTestable& shaderTestable)
    {
        EXPECT_TRUE(shaderTestable.getID() > 0);
        EXPECT_NO_THROW(shaderTestable.use());
        EXPECT_NO_THROW(shaderTestable.setBool("uniform", 1));
        EXPECT_NO_THROW(shaderTestable.setInt("uniform", 1));
        EXPECT_NO_THROW(shaderTestable.setFloat("uniform", 1.0f));
        EXPECT_NO_THROW(shaderTestable.setVec2("uniform", std::vector<float>{1.0f, 1.0f}.data()));
        EXPECT_NO_THROW(shaderTestable.setVec3("uniform", std::vector<float>{1.0f, 1.0f, 1.0f}.data()));
        EXPECT_NO_THROW(shaderTestable.setVec4("uniform", std::vector<float>{1.0f, 1.0f, 1.0f, 1.0f}.data()));
        EXPECT_NO_THROW(shaderTestable.setMat2("uniform", std::vector<float>{1.0f, 1.0f,
            1.0f, 1.0f}.data()));
        EXPECT_NO_THROW(shaderTestable.setMat3("uniform", std::vector<float>{1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f}.data()));
        EXPECT_NO_THROW(shaderTestable.setMat4("uniform", std::vector<float>{1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f}.data()));
    }
};

// ------------------------------------------------------------------------
TEST_F(ShaderTest, CreateShaderWithGeometryShaderCorrectlyCreatesNewShaderWithValidIDAndNonThrowingUniformSetterCalls)
{
    ShaderTestable shaderTestable("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
    verifyShaderOperations(shaderTestable);
}

// ------------------------------------------------------------------------
TEST_F(ShaderTest, CreateShaderWithNoGeometryShaderCorrectlyCreatesNewShaderWithValidIDAndNonThrowingUniformSetterCalls)
{
    ShaderTestable shaderTestable("ztest_valid_shader.vert", "ztest_valid_shader.frag");
    verifyShaderOperations(shaderTestable);
}

// ------------------------------------------------------------------------
TEST_F(ShaderTest, NonExistantShaderNameThrowsExpection)
{
    EXPECT_THROW(ShaderTestable shaderTestable("", "", ""), std::runtime_error);
}

// ------------------------------------------------------------------------
TEST_F(ShaderTest, CompilationErrorShaderThrowsExpection)
{
    EXPECT_THROW(ShaderTestable shaderTestable("ztest_invalid_shader.vert", "ztest_invalid_shader.frag", "ztest_invalid_shader.geom"), std::runtime_error);
}
