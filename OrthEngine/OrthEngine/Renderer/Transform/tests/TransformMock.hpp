#ifndef TRANSFORMMOCK_HPP
#define TRANSFORMMOCK_HPP

#include <gmock/gmock.h>

#include "Transform/Transform.hpp"

class TransformMock : public Transform
{
public:
    MOCK_METHOD(void, setPerspective, (const float, const float, const float, const float), (override));
    MOCK_METHOD(void, setOrthographic, (const MathUtils::Vec2&, const MathUtils::Vec2&, const float, const float), (override));
    MOCK_METHOD(void, applyTranslation, (const MatrixTransform::MatrixTypes, const MathUtils::Vec3&), (override));
    MOCK_METHOD(void, applyRotation, (const MatrixTransform::MatrixTypes, const float, const MathUtils::Vec3&), (override));
    MOCK_METHOD(void, applyScale, (const MatrixTransform::MatrixTypes, const MathUtils::Vec3&), (override));
    MOCK_METHOD(void, resetMatrix, (const MatrixTransform::MatrixTypes), (override));
    MOCK_METHOD((std::array<float, MathUtils::MAT4_SIZE>), getTransformationMatrix, (const MatrixTransform::MatrixTypes), (const, override));
};

#endif // TRANSFORMMOCK_HPP
