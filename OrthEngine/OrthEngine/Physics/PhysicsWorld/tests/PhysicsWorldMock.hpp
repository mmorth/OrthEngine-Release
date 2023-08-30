#ifndef PHYSICSWORLDMOCK_HPP
#define PHYSICSWORLDMOCK_HPP

#include <gmock/gmock.h>

#include "PhysicsWorld.hpp"

class PhysicsWorldMock : public PhysicsWorld
{
public:
    MOCK_METHOD(void, addRigidBody, (RigidBody* body), (override));
    MOCK_METHOD(void, removeRigidBody, (RigidBody* body), (override));
    MOCK_METHOD(void, stepSimulation, (float deltaTime, bool performCollisionDetection), (override));
    MOCK_METHOD(MathUtils::Vec3, detectCameraCollision, (const MathUtils::Vec3& playerPosition, const MathUtils::Vec3& cameraPosition, const MathUtils::Vec3& cameraFront), (override));
};

#endif // PHYSICSWORLDMOCK_HPP
