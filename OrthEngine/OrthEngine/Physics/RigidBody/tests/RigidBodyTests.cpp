#include <gtest/gtest.h>

#include "RigidBodyImpl.hpp"

// ===============================================================
// RigidBodyImplTestable
// ===============================================================
class RigidBodyImplTestable : public RigidBodyImpl
{
public:
    RigidBodyImplTestable(const PhysicsShapeInfo& shape, const ObjectLocationOrientation& startTransform)
        : RigidBodyImpl(shape, startTransform) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    btBoxShape* getBoxShape() { return m_boxShape; };
    btDefaultMotionState* getMotionState() { return m_motionState; };
};

// ------------------------------------------------------------------------
class RigidBodyImplTest : public testing::Test
{

};

// ------------------------------------------------------------------------
TEST_F(RigidBodyImplTest, RigidBodyConstructorProperlySetsMembers)
{
    PhysicsProperties properties(1.0f, 1.0f);
    PhysicsShapeInfo shapeInfo(ShapeType::Box, properties);
    MathUtils::Vec3 position(1.0f, 1.0f, 1.0f);
    MathUtils::Vec3 scale(2.0f, 2.0f, 2.0f);
    MathUtils::Vec3 rotationAxis(3.0f, 3.0f, 3.0f);
    float orientation = 4.0f;
    ObjectLocationOrientation startTransform(position, scale, rotationAxis, orientation);

    // FUT
    std::unique_ptr<RigidBodyImpl> rigidBody = std::make_unique<RigidBodyImpl>(shapeInfo, startTransform);
    
    EXPECT_EQ(rigidBody->getBtRigidBody()->getMass(), properties.mass);
    EXPECT_EQ(rigidBody->getBtRigidBody()->getFriction(), properties.friction);
    EXPECT_EQ(rigidBody->getPosition(), position);
    
    // Confirm box shape and half extents
    btCollisionShape* collisionShape = rigidBody->getBtRigidBody()->getCollisionShape();
    EXPECT_EQ(collisionShape->getShapeType(), BOX_SHAPE_PROXYTYPE);
    EXPECT_EQ(collisionShape->getLocalScaling(), BtConversions::mathToBtVec3(scale * 0.5f));
}

// ------------------------------------------------------------------------
TEST_F(RigidBodyImplTest, RigidBodyGetSetPossitionCorrectlyUpdatesAndReturnsMemberVariables)
{
    PhysicsShapeInfo shapeInfo;
    ObjectLocationOrientation startTransform;
    std::unique_ptr<RigidBodyImpl> rigidBody = std::make_unique<RigidBodyImpl>(shapeInfo, startTransform);

    // FUT
    MathUtils::Vec3 newPosition(1.0f, 2.0f, 3.0f);
    rigidBody->setPosition(newPosition);

    EXPECT_EQ(rigidBody->getPosition(), newPosition);
}

// ------------------------------------------------------------------------
TEST_F(RigidBodyImplTest, RigidBodyMoveToPositionProperlySetsObjectVelocity)
{
    PhysicsShapeInfo shapeInfo;
    ObjectLocationOrientation startTransform;
    std::unique_ptr<RigidBodyImpl> rigidBody = std::make_unique<RigidBodyImpl>(shapeInfo, startTransform);

    // FUT
    MathUtils::Vec3 newPosition(5.0f, 5.0f, 5.0f);
    rigidBody->moveToPosition(newPosition, 1.0f);

    btVector3 velocity = rigidBody->getBtRigidBody()->getLinearVelocity();
    EXPECT_EQ(rigidBody->getBtRigidBody()->getLinearVelocity(), BtConversions::mathToBtVec3(newPosition));
}
