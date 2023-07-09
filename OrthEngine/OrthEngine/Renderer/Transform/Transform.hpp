#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <array>
#include <iostream>
#include <string>

//#include <g3log/g3log.hpp>
//#include <g3log/loglevels.hpp>
//#include <g3log/logworker.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MathUtils.hpp"

namespace MatrixTransform 
{
    enum class MatrixTypes 
    {
        MODEL = 1,
        VIEW = 2,
        PROJECTION = 3
    };
}

class Transform {
public:
    Transform();

    virtual void setPerspective(const float fov, const float aspectRatio, const float zNear, const float zFar);
    virtual void setOrthographic(const MathUtils::Vec2& widthRange, const MathUtils::Vec2& heightRange, const float zNear, const float zFar);

    virtual void applyTranslation(const MatrixTransform::MatrixTypes matrixType, const MathUtils::Vec3& translationVector);
    virtual void applyRotation(const MatrixTransform::MatrixTypes matrixType, const float rotationAngle, const MathUtils::Vec3& rotationAxis);
    virtual void applyScale(const MatrixTransform::MatrixTypes matrixType, const MathUtils::Vec3& scaleMatrix);

    virtual void resetMatrix(const MatrixTransform::MatrixTypes matrixType);
    virtual std::array<float, MathUtils::MAT4_SIZE> getTransformationMatrix(const MatrixTransform::MatrixTypes matrixType) const;

private:
    void copyMatrixToArray(const glm::mat4& matrix, std::array<float, MathUtils::MAT4_SIZE>& matrixArray) const;

    glm::mat4 modelMatrix;
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;

    static constexpr glm::mat4 IDENTITY_MATRIX = glm::mat4(1.0f);
};

#endif // TRANSFORM_HPP
