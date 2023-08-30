#ifndef RIGIDBODY_HPP
#define RIGIDBODY_HPP

#include "BulletTypeConversions.hpp"
#include "MathUtils.hpp"
#include "ObjectStructs.hpp"

#include "RigidBodyImpl.hpp"

class RigidBody
{
public:
    RigidBody(const PhysicsShapeInfo& shape, const ObjectLocationOrientation& startTransform);
    ~RigidBody();

    // Rigid object properties getters/setters
    virtual void moveToPosition(const MathUtils::Vec3& position, const float timeToReach);
    virtual void setPosition(const MathUtils::Vec3& position);
    virtual MathUtils::Vec3 getPosition() const;

    // Get the underlying btRigidBody
    virtual btRigidBody* getBtRigidBody() const;

protected:
    RigidBodyImpl* m_pimpl;
};

#endif // RIGIDBODY_HPP
