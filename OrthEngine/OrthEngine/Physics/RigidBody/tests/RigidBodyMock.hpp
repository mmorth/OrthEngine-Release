#ifndef RIGIDBODYMOCK_HPP
#define RIGIDBODYMOCK_HPP

#include <gmock/gmock.h>

#include "RigidBody.hpp"

class RigidBodyMock : public RigidBody 
{
public:
    RigidBodyMock(const PhysicsShapeInfo& shape, const ObjectLocationOrientation& startTransform)
        : RigidBody(shape, startTransform) {}

    MOCK_METHOD(void, moveToPosition, (const MathUtils::Vec3& position, const float timeToReach), (override));
    MOCK_METHOD(void, setPosition, (const MathUtils::Vec3& position), (override));
    MOCK_METHOD(MathUtils::Vec3, getPosition, (), (const, override));
    MOCK_METHOD(btRigidBody*, getBtRigidBody, (), (const, override));
};

#endif // RIGIDBODYMOCK_HPP