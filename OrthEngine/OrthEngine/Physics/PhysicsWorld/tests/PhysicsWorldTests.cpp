#include <gtest/gtest.h>

#include <bullet/btBulletDynamicsCommon.h>
#include "PhysicsWorldImpl.hpp"

// ===============================================================
// PhysicsWorldTestable
// ===============================================================
class PhysicsWorldImplTestable : public PhysicsWorldImpl
{
public:
    PhysicsWorldImplTestable()
        : PhysicsWorldImpl() {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    btDefaultCollisionConfiguration* getCollisionConfiguration() { return m_collisionConfiguration; };
    btCollisionDispatcher* getDispatcher() { return m_dispatcher; }
    btDbvtBroadphase* getBroadphase() { return m_broadphase; }
    btSequentialImpulseConstraintSolver* getSolver() { return m_solver; }
    btDiscreteDynamicsWorld* getDynamicWorld() { return m_dynamicsWorld; }
};

// ------------------------------------------------------------------------
class PhysicsWorldTest : public testing::Test
{

};

// ------------------------------------------------------------------------
TEST_F(PhysicsWorldTest, PhysicsWorldConstructorProperlySetsMembers)
{
    // FUT
    std::unique_ptr<PhysicsWorldImplTestable> physicsWorldImplTestable = std::make_unique<PhysicsWorldImplTestable>();

    EXPECT_NE(physicsWorldImplTestable->getCollisionConfiguration(), nullptr);
    EXPECT_NE(physicsWorldImplTestable->getDispatcher(), nullptr);
    EXPECT_NE(physicsWorldImplTestable->getBroadphase(), nullptr);
    EXPECT_NE(physicsWorldImplTestable->getSolver(), nullptr);
    EXPECT_NE(physicsWorldImplTestable->getDynamicWorld(), nullptr);
}

// ------------------------------------------------------------------------
TEST_F(PhysicsWorldTest, PhysicsWorldAddAndRemoveRigidBodyCorrectlyUpdatesDynamicWorldRigidBodyList)
{
    std::unique_ptr<PhysicsWorldImplTestable> physicsWorldImplTestable = std::make_unique<PhysicsWorldImplTestable>();

    btCollisionShape* collisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    btDefaultMotionState* motionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 0, 0)));
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, collisionShape);
    btRigidBody* rigidBody = new btRigidBody(rbInfo);

    // FUTs
    physicsWorldImplTestable->addRigidBody(rigidBody);
    EXPECT_EQ(physicsWorldImplTestable->getDynamicWorld()->getNumCollisionObjects(), 1);
    physicsWorldImplTestable->removeRigidBody(rigidBody);
    EXPECT_EQ(physicsWorldImplTestable->getDynamicWorld()->getNumCollisionObjects(), 0);

    delete rigidBody;
    delete collisionShape;
    delete motionState;
}

TEST_F(PhysicsWorldTest, DetectCameraCollisionProperlyHandlesPositiveCollision) 
{
    // Configure physics world with a collision
    std::unique_ptr<PhysicsWorldImplTestable> physicsWorldImplTestable = std::make_unique<PhysicsWorldImplTestable>();
    btCollisionShape* collisionShape = new btBoxShape(btVector3(1.0f, 1.0f, 1.0f));
    btVector3 position(5.0f, 5.0f, 5.0f);
    btTransform transform; transform.setIdentity();
    transform.setOrigin(position);
    btDefaultMotionState* motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0f, motionState, collisionShape);
    btRigidBody* rigidBody = new btRigidBody(rbInfo);
    physicsWorldImplTestable->addRigidBody(rigidBody);

    // Perform positive collision test
    btVector3 playerPosition(0.0f, 0.0f, 0.0f);
    btVector3 cameraPosition(10.0f, 10.0f, 10.0f);
    btVector3 cameraFront(0.0f, 0.0f, -1.0f); // Assuming camera is looking forward

    MathUtils::Vec3 newCameraPosition = physicsWorldImplTestable->detectCameraCollision(playerPosition, cameraPosition, cameraFront);

    // Assert that newCameraPosition is not equal to the initial cameraPosition
    ASSERT_NE(newCameraPosition, BtConversions::btToMathVec3(position));
    
    physicsWorldImplTestable->removeRigidBody(rigidBody);
    delete rigidBody;
    delete collisionShape;
    delete motionState;
}

TEST_F(PhysicsWorldTest, DetectCameraCollisionDoesNotHandleWhenNoCollision) 
{
    // Configure physics world
    std::unique_ptr<PhysicsWorldImplTestable> physicsWorldImplTestable = std::make_unique<PhysicsWorldImplTestable>();
    btVector3 playerPosition(0.0f, 0.0f, 0.0f);
    btVector3 cameraPosition(2.0f, 2.0f, 2.0f);
    btVector3 cameraFront(0.0f, 0.0f, -1.0f);

    // FUT
    MathUtils::Vec3 newCameraPosition = physicsWorldImplTestable->detectCameraCollision(playerPosition, cameraPosition, cameraFront);

    // No collision, so position should not change
    ASSERT_EQ(newCameraPosition, MathUtils::Vec3(cameraPosition.x(), cameraPosition.y(), cameraPosition.z()));
}
