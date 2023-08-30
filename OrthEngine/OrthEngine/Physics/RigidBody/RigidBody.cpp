#include "RigidBody.hpp"

// ------------------------------------------------------------------------
RigidBody::RigidBody(const PhysicsShapeInfo& shape, const ObjectLocationOrientation& startTransform)
    : m_pimpl(new RigidBodyImpl(shape, startTransform))
{}

// ------------------------------------------------------------------------
RigidBody::~RigidBody()
{
    delete m_pimpl;
}

// ------------------------------------------------------------------------
void RigidBody::moveToPosition(const MathUtils::Vec3& position, const float timeToReach) 
{ 
    m_pimpl->moveToPosition(position, timeToReach); 
}

// ------------------------------------------------------------------------
void RigidBody::setPosition(const MathUtils::Vec3& position) 
{ 
    m_pimpl->setPosition(position); 
}

// ------------------------------------------------------------------------
MathUtils::Vec3 RigidBody::getPosition() const 
{ 
    return m_pimpl->getPosition(); 
}

// ------------------------------------------------------------------------
btRigidBody* RigidBody::getBtRigidBody() const 
{ 
    return m_pimpl->getBtRigidBody(); 
}
