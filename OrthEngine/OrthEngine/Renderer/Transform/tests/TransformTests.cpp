#include <gtest/gtest.h>
#include "Transform/Transform.hpp"

// ------------------------------------------------------------------------
class TransformTest : public testing::Test
{
protected:
    virtual void SetUp()
    {}

    virtual void TearDown()
    {}

    void TestMatricesDefaultToIdentity(MatrixTransform::MatrixTypes matrixType);
    void TestTraslationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes matrixType);
    void TestScaleCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes matrixType);
    void TestRotationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes matrixType);
    void TestPerspectiveProjectionCorrectlySetToProjectionMatrix();
    void ChangeModelViewAndProjectionMatrixDefaults();

    std::array<float, MathUtils::MAT4_SIZE> IDENTITY_MATRIX
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
};


// ------------------------------------------------------------------------
void TransformTest::TestMatricesDefaultToIdentity(MatrixTransform::MatrixTypes matrixType)
{
    Transform transform;
    // FUT
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(matrixType);
    EXPECT_EQ(IDENTITY_MATRIX, actual);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, ResetModelMatrixProperlySetsToIdentityMatrix)
{
    TestMatricesDefaultToIdentity(MatrixTransform::MatrixTypes::MODEL);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, ResetViewMatrixProperlySetsToIdentityMatrix)
{
    TestMatricesDefaultToIdentity(MatrixTransform::MatrixTypes::VIEW);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, ResetProjectionMatrixProperlySetsToIdentityMatrix)
{
    TestMatricesDefaultToIdentity(MatrixTransform::MatrixTypes::PROJECTION);
}

// ------------------------------------------------------------------------
void TransformTest::TestTraslationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes matrixType)
{
    MathUtils::Vec3 translation{ 3.0f, 3.0f, 3.0f };
    std::array<float, MathUtils::MAT4_SIZE> expected = 
    {
            1, 0, 0, 0,
            0, 1, 0, 0,
            0, 0, 1, 0,
            translation.x, translation.y, translation.z, 1
    
    };

    Transform transform;
    // FUT
    transform.applyTranslation(matrixType, translation);
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(matrixType);

    EXPECT_EQ(expected, actual);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, TranslationCorrectlyAppliedToModelMatrix)
{
    TestTraslationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes::MODEL);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, TranslationCorrectlyAppliedToViewMatrix)
{
    TestTraslationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes::VIEW);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, TranslationOnProjectionParameterThrowsInvalidArgument)
{
    MathUtils::Vec3 translation{ 3.0f, 3.0f, 3.0f };

    Transform transform;
    // FUT
    EXPECT_THROW(transform.applyTranslation(MatrixTransform::MatrixTypes::PROJECTION, translation), std::invalid_argument);
}

// ------------------------------------------------------------------------
void TransformTest::TestScaleCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes matrixType)
{
    MathUtils::Vec3 scale{ 3.0f, 3.0f, 3.0f };
    std::array<float, MathUtils::MAT4_SIZE> expected =
    {
            scale.x, 0, 0, 0,
            0, scale.y, 0, 0,
            0, 0, scale.z, 0,
            0, 0, 0, 1

    };

    Transform transform;
    // FUT
    transform.applyScale(matrixType, scale);
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(matrixType);

    EXPECT_EQ(expected, actual);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, ScaleCorrectlyAppliedToModelMatrix)
{
    TestScaleCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes::MODEL);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, ScaleCorrectlyAppliedToViewMatrix)
{
    TestScaleCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes::VIEW);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, ScaleOnProjectionParameterThrowsInvalidArgument)
{
    MathUtils::Vec3 scale{ 3.0f, 3.0f, 3.0f };

    Transform transform;
    // FUT
    EXPECT_THROW(transform.applyScale(MatrixTransform::MatrixTypes::PROJECTION, scale), std::invalid_argument);
}

// ------------------------------------------------------------------------
void TransformTest::TestRotationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes matrixType)
{
    float rotationAngle = 45.0f;
    MathUtils::Vec3 rotationAxis{ 1.0f, 1.0f, 1.0f };
    std::array<float, MathUtils::MAT4_SIZE> expected =  
    {
            0.804737866f, 0.505879343f, -0.310617208f, 0.0f,
            - 0.310617208f, 0.804737866f, 0.505879343f, 0.00f,
            0.505879343f, -0.310617208f, 0.804737866f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f

    };

    Transform transform;
    // FUT
    transform.applyRotation(matrixType, rotationAngle, rotationAxis);
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(matrixType);

    EXPECT_EQ(expected, actual);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, RotationCorrectlyAppliedToModelMatrix)
{
    TestRotationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes::MODEL);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, RotationCorrectlyAppliedToViewMatrix)
{
    TestRotationCorrectlyAppliedToMatrix(MatrixTransform::MatrixTypes::VIEW);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, RotationOnProjectionParameterThrowsInvalidArgument)
{
    float rotationAngle = 45.0f;
    MathUtils::Vec3 rotationAxis{ 1.0f, 1.0f, 1.0f };
    
    Transform transform;
    // FUT
    EXPECT_THROW(transform.applyRotation(MatrixTransform::MatrixTypes::PROJECTION, rotationAngle, rotationAxis), std::invalid_argument);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, RotationOnZeroRotationVectorDoesNotOperate)
{
    float rotationAngle = 45.0f;
    MathUtils::Vec3 rotationAxis{ 0.0f, 0.0f, 0.0f };

    Transform transform;
    // FUT
    transform.applyRotation(MatrixTransform::MatrixTypes::MODEL, rotationAngle, rotationAxis);
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::MODEL);

    EXPECT_EQ(IDENTITY_MATRIX, actual);
}

// ------------------------------------------------------------------------
void TransformTest::TestPerspectiveProjectionCorrectlySetToProjectionMatrix()
{
    float fov = 45.0f;
    float aspectRatio = 1200.0f / 800.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;

    std::array<float, MathUtils::MAT4_SIZE> expected =
    {
            1.60947561f, 0, 0, 0,
            0, 2.41421342f, 0, 0,
            0, 0, -1.00200200f, -1.0f,
            0, 0, -0.200200200f, 0

    };

    Transform transform;
    // FUT
    transform.setPerspective(fov, aspectRatio, zNear, zFar);
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::PROJECTION);

    EXPECT_EQ(expected, actual);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, PerspectiveProjectionCorrectlySetToProjectionMatrix)
{
    TestPerspectiveProjectionCorrectlySetToProjectionMatrix();
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, OrthographicProjectionCorrectlySetToProjectionMatrix)
{
    MathUtils::Vec2 widthRange{ 0.0f, 1200.0f };
    MathUtils::Vec2 heightRange{ 0.0f, 800.0f };
    float zNear = 0.1f;
    float zFar = 100.0f;

    std::array<float, MathUtils::MAT4_SIZE> expected =
    {
            0.00166666671f, 0, 0, 0,
            0, 0.00249999994f, 0, 0,
            0, 0, -0.0200200192f, 0,
            -1.0f, -1.0f, -1.00200200f, 1.0f

    };

    Transform transform;
    // FUT
    transform.setOrthographic(widthRange, heightRange, zNear, zFar);
    std::array<float, MathUtils::MAT4_SIZE> actual = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::PROJECTION);

    EXPECT_EQ(expected, actual);
}

// ------------------------------------------------------------------------
TEST_F(TransformTest, MatrixResetMakesModelViewAndProjectionIdentityMatrix)
{
    float fov = 45.0f;
    float aspectRatio = 1200.0f / 800.0f;
    float zNear = 0.1f;
    float zFar = 100.0f;
    MathUtils::Vec3 translation{ 3.0f, 3.0f, 3.0f };
    Transform transform;
    transform.applyTranslation(MatrixTransform::MatrixTypes::MODEL, translation);
    transform.applyTranslation(MatrixTransform::MatrixTypes::VIEW, translation);
    transform.setPerspective(fov, aspectRatio, zNear, zFar);

    // FUT
    transform.resetMatrix(MatrixTransform::MatrixTypes::MODEL);
    transform.resetMatrix(MatrixTransform::MatrixTypes::VIEW);
    transform.resetMatrix(MatrixTransform::MatrixTypes::PROJECTION);

    std::array<float, MathUtils::MAT4_SIZE> modelMatrix = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::MODEL);
    std::array<float, MathUtils::MAT4_SIZE> viewMatrix = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::VIEW);
    std::array<float, MathUtils::MAT4_SIZE> projectionMatrix = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::PROJECTION);

    ASSERT_EQ(IDENTITY_MATRIX, modelMatrix);
    ASSERT_EQ(IDENTITY_MATRIX, viewMatrix);
    ASSERT_EQ(IDENTITY_MATRIX, projectionMatrix);
}
