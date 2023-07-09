#include "Transform.hpp"

// ------------------------------------------------------------------------
Transform::Transform()
    : modelMatrix(IDENTITY_MATRIX)
    , viewMatrix(IDENTITY_MATRIX)
    , projectionMatrix(IDENTITY_MATRIX)
{
    //LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
void Transform::setPerspective(const float fov, const float aspectRatio, const float zNear, const float zFar)
{
    projectionMatrix = glm::perspective(glm::radians(fov), aspectRatio, zNear, zFar);

    /*LOG(INFO) << "Perspective set with fov: " << fov << ", aspectRatio: " << aspectRatio
        << ", zNear: " << zNear << ", zFar: " << zFar;*/
}

// ------------------------------------------------------------------------
void Transform::setOrthographic(const MathUtils::Vec2& widthRange, const MathUtils::Vec2& heightRange, const float zNear, const float zFar)
{
    projectionMatrix = glm::ortho(widthRange.x, widthRange.y, heightRange.x, heightRange.y, zNear, zFar);

    /*LOG(INFO) << "Orthographic projection set with widthRange: (" << widthRange.x << ", " << widthRange.y
        << "), heightRange: (" << heightRange.x << ", " << heightRange.y
        << "), zNear: " << zNear << ", zFar: " << zFar;*/
}

// ------------------------------------------------------------------------
void Transform::applyTranslation(const MatrixTransform::MatrixTypes matrixType, const MathUtils::Vec3& translationVector)
{
    switch (matrixType)
    {
        case MatrixTransform::MatrixTypes::MODEL:
            modelMatrix = glm::translate(modelMatrix, glm::vec3(translationVector.x, translationVector.y, translationVector.z));
            //LOG(INFO) << "Applied translation to MODEL matrix with vector: (" << translationVector.x << ", "
            //    << translationVector.y << ", " << translationVector.z << ")";
            break;
        case MatrixTransform::MatrixTypes::VIEW:
            viewMatrix = glm::translate(viewMatrix, glm::vec3(translationVector.x, translationVector.y, translationVector.z));
            //LOG(INFO) << "Applied translation to VIEW matrix with vector: (" << translationVector.x << ", "
            //    << translationVector.y << ", " << translationVector.z << ")";
            break;
        default:
            throw std::invalid_argument("Invalid matrix type for translation operation: " + std::to_string(static_cast<int>(matrixType)));
    }
}

// ------------------------------------------------------------------------
void Transform::applyRotation(const MatrixTransform::MatrixTypes matrixType, const float rotationAngle, const MathUtils::Vec3& rotationAxis)
{
    // rotation axis must be non-zero vector
    if (rotationAxis.length() < glm::epsilon<float>())
        return; // TODO: Can move this back to a throw once you update the rotational logic to be more accurate (e.g. not hardcoding using position as axis of rotation)

    switch (matrixType)
    {
        case MatrixTransform::MatrixTypes::MODEL:
            modelMatrix = glm::rotate(modelMatrix, glm::radians(rotationAngle), glm::vec3(rotationAxis.x, rotationAxis.y, rotationAxis.z));
            //LOG(INFO) << "Applied rotation to MODEL matrix with angle: " << rotationAngle
            //    << " and axis: (" << rotationAxis.x << ", " << rotationAxis.y << ", " << rotationAxis.z << ")";
            break;
        case MatrixTransform::MatrixTypes::VIEW:
            viewMatrix = glm::rotate(viewMatrix, glm::radians(rotationAngle), glm::vec3(rotationAxis.x, rotationAxis.y, rotationAxis.z));
            //LOG(INFO) << "Applied rotation to VIEW matrix with angle: " << rotationAngle
            //    << " and axis: (" << rotationAxis.x << ", " << rotationAxis.y << ", " << rotationAxis.z << ")";
            break;
        default:
            throw std::invalid_argument("Invalid matrix type for rotation operation: " + std::to_string(static_cast<int>(matrixType)));
    }
}

// ------------------------------------------------------------------------
void Transform::applyScale(const MatrixTransform::MatrixTypes matrixType, const MathUtils::Vec3& scaleMatrix)
{
    switch (matrixType)
    {
        case MatrixTransform::MatrixTypes::MODEL:
            modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleMatrix.x, scaleMatrix.y, scaleMatrix.z));
            //LOG(INFO) << "Applied scale to MODEL matrix with scale factors: (" << scaleMatrix.x << ", "
            //    << scaleMatrix.y << ", " << scaleMatrix.z << ")";
            break;
        case MatrixTransform::MatrixTypes::VIEW:
            viewMatrix = glm::scale(viewMatrix, glm::vec3(scaleMatrix.x, scaleMatrix.y, scaleMatrix.z));
            //LOG(INFO) << "Applied scale to VIEW matrix with scale factors: (" << scaleMatrix.x << ", "
            //    << scaleMatrix.y << ", " << scaleMatrix.z << ")";
            break;
        default:
            throw std::invalid_argument("Invalid matrix type for scale operation: " + std::to_string(static_cast<int>(matrixType)));
    }
}

// ------------------------------------------------------------------------
void Transform::resetMatrix(const MatrixTransform::MatrixTypes matrixType)
{
    switch (matrixType)
    {
        case MatrixTransform::MatrixTypes::MODEL:
            modelMatrix = IDENTITY_MATRIX;
            //LOG(INFO) << "Reset MODEL matrix";
            break;
        case MatrixTransform::MatrixTypes::VIEW:
            viewMatrix = IDENTITY_MATRIX;
            //LOG(INFO) << "Reset VIEW matrix";
            break;
        case MatrixTransform::MatrixTypes::PROJECTION:
            projectionMatrix = IDENTITY_MATRIX;
            //LOG(INFO) << "Reset PROJECTION matrix";
            break;
        default:
            //LOG(WARNING) << "No reset operation defined for matrix type: " << static_cast<int>(matrixType);
            break;
    }
}

// ------------------------------------------------------------------------
std::array<float, MathUtils::MAT4_SIZE> Transform::getTransformationMatrix(const MatrixTransform::MatrixTypes matrixType) const
{
    std::array<float, MathUtils::MAT4_SIZE> matrixArray{};

    switch (matrixType)
    {
        case MatrixTransform::MatrixTypes::MODEL:
            copyMatrixToArray(modelMatrix, matrixArray);
            //LOG(INFO) << "Retrieved MODEL matrix";
            break;
        case MatrixTransform::MatrixTypes::VIEW:
            copyMatrixToArray(viewMatrix, matrixArray);
            //LOG(INFO) << "Retrieved VIEW matrix";
            break;
        case MatrixTransform::MatrixTypes::PROJECTION:
            copyMatrixToArray(projectionMatrix, matrixArray);
            //LOG(INFO) << "Retrieved PROJECTION matrix";
            break;
        default:
            throw std::invalid_argument("Invalid matrix type for matrix get operation: " + std::to_string(static_cast<int>(matrixType)));
    }

    return matrixArray;
}

// ------------------------------------------------------------------------
void Transform::copyMatrixToArray(const glm::mat4& matrix, std::array<float, MathUtils::MAT4_SIZE>& matrixArray) const
{
    memcpy(matrixArray.data(), glm::value_ptr(matrix), sizeof(float) * MathUtils::MAT4_SIZE);
}
