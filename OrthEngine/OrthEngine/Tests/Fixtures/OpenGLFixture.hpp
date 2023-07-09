#ifndef OPENGLTESTFIXTURE_HPP
#define OPENGLTESTFIXTURE_HPP

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

class OpenGLTestFixture : public ::testing::Test
{
protected:
    static GLFWwindow* window;

    static void SetUpTestCase();
    static void TearDownTestCase();
};

// TODO: Determine a more centralized location for these
MATCHER_P(FloatArrayEq, expected, "Float array is equal")
{
    const float* actual = arg;
    for (size_t i = 0; i < MathUtils::MAT4_SIZE; ++i)
    {
        if (actual[i] != expected[i])
            return false;
    }
    return true;
}

MATCHER_P(FloatVec3Eq, expected, "Float Vec3 is equal")
{
    const float* actual = arg;
    for (size_t i = 0; i < MathUtils::VEC3_SIZE; ++i)
    {
        if (actual[i] != expected[i])
            return false;
    }
    return true;
}

MATCHER_P(FloatVectorArrayEq, expected, "Float vector array is equal")
{
    std::array<float, MathUtils::MAT4_SIZE> actual = arg.at(0);
    for (size_t i = 0; i < MathUtils::MAT4_SIZE; ++i)
    {
        if (actual[i] != expected[0][i])
            return false;
    }
    return true;
}

MATCHER_P(FloatVectorEq, expected, "Float array is equal")
{
    std::vector<float> actual = arg;
    for (size_t i = 0; i < actual.size() && i < expected.size(); ++i)
    {
        if (actual[i] != expected[i])
            return false;
    }
    return actual.size() == expected.size();
}

#endif // OPENGLTESTFIXTURE_HPP
