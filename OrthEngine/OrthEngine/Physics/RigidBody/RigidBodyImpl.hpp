#ifndef RIGIDBODYIMPL_HPP
#define RIGIDBODYIMPL_HPP

#include <iostream>

#include <bullet/btBulletDynamicsCommon.h>

#include "BulletTypeConversions.hpp"
#include "MathUtils.hpp"
#include "ObjectStructs.hpp"

class RigidBodyImpl
{
public:
    RigidBodyImpl(const PhysicsShapeInfo& shape, const ObjectLocationOrientation& startTransform);
    ~RigidBodyImpl();

    // Rigid object properties getters/setters
    virtual void moveToPosition(const MathUtils::Vec3& position, const float timeToReach);
    virtual void setPosition(const MathUtils::Vec3& position);
    virtual MathUtils::Vec3 getPosition() const;

    // Get the underlying btRigidBody
    btRigidBody* getBtRigidBody() const { return m_rigidBody; }

protected:
    btRigidBody* m_rigidBody;
    btBoxShape* m_boxShape;
    btDefaultMotionState* m_motionState;
};

#endif // RIGIDBODYIMPL_HPP
