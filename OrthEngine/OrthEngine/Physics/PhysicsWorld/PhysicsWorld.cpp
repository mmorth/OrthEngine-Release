#include "PhysicsWorld.hpp"

// ------------------------------------------------------------------------
PhysicsWorld::PhysicsWorld()
    : m_pimpl(new PhysicsWorldImpl())
{}

// ------------------------------------------------------------------------
PhysicsWorld::~PhysicsWorld()
{
    delete m_pimpl;
}

// Add/remove Rigid Body Functions
void PhysicsWorld::addRigidBody(RigidBody* body) { m_pimpl->addRigidBody(body->getBtRigidBody()); }
void PhysicsWorld::removeRigidBody(RigidBody* body) { m_pimpl->removeRigidBody(body->getBtRigidBody()); }

// Collision Loop Functions
void PhysicsWorld::stepSimulation(const float deltaTime, bool performCollisionDetection) { m_pimpl->stepSimulation(deltaTime, performCollisionDetection); }

// ------------------------------------------------------------------------
MathUtils::Vec3 PhysicsWorld::detectCameraCollision(const MathUtils::Vec3& playerPosition, const MathUtils::Vec3& cameraPosition, const MathUtils::Vec3& cameraFront)
{
    return m_pimpl->detectCameraCollision(BtConversions::mathToBtVec3(playerPosition), BtConversions::mathToBtVec3(cameraPosition), BtConversions::mathToBtVec3(cameraFront));
}
