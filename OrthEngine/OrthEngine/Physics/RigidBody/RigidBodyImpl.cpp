#include "RigidBodyImpl.hpp"

// ------------------------------------------------------------------------
RigidBodyImpl::RigidBodyImpl(const PhysicsShapeInfo& shape, const ObjectLocationOrientation& startTransform)
{
    m_boxShape = new btBoxShape(BtConversions::mathToBtVec3(startTransform.scale * 0.5f));
    btTransform transform; transform.setIdentity();
    transform.setOrigin(BtConversions::mathToBtVec3(startTransform.position));

    btVector3 localInertia;
    if (shape.physicsProperties.mass != 0.0f)
        m_boxShape->calculateLocalInertia(shape.physicsProperties.mass, localInertia);

    m_motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rbInfo(shape.physicsProperties.mass, m_motionState, m_boxShape, localInertia);
    m_rigidBody = new btRigidBody(rbInfo);
    m_rigidBody->setFriction(shape.physicsProperties.friction);

    // TODO: Make this optional to improve performance of objects that do not move
    m_rigidBody->setActivationState(DISABLE_DEACTIVATION);
}

// ------------------------------------------------------------------------
RigidBodyImpl::~RigidBodyImpl()
{
    delete m_rigidBody;
    delete m_motionState;
    delete m_boxShape;
}

// ------------------------------------------------------------------------
void RigidBodyImpl::moveToPosition(const MathUtils::Vec3& position, const float timeToReach)
{
    btVector3 targetPosition = BtConversions::mathToBtVec3(position);
    btVector3 currentPosition = m_rigidBody->getWorldTransform().getOrigin();
    btVector3 displacement = targetPosition - currentPosition;
    btVector3 desiredVelocity = displacement / timeToReach;
    m_rigidBody->setLinearVelocity(desiredVelocity);
}

// ------------------------------------------------------------------------
void RigidBodyImpl::setPosition(const MathUtils::Vec3& position)
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    transform.setOrigin(BtConversions::mathToBtVec3(position));
    m_motionState->setWorldTransform(transform);
}

// ------------------------------------------------------------------------
MathUtils::Vec3 RigidBodyImpl::getPosition() const
{
    btTransform transform;
    m_motionState->getWorldTransform(transform);
    return BtConversions::btToMathVec3(transform.getOrigin());
}
