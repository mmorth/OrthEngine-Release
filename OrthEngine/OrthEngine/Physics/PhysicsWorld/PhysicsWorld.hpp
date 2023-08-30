#ifndef PHYSICSWORLD_HPP
#define PHYSICSWORLD_HPP

#include "RigidBody.hpp"
#include "ObjectStructs.hpp"
#include "PhysicsWorldImpl.hpp"

class PhysicsWorld
{
public:
    PhysicsWorld();
    ~PhysicsWorld();

    // Methods for adding and removing collision shapes and rigid bodies to the world
    virtual void addRigidBody(RigidBody* body);
    virtual void removeRigidBody(RigidBody* body);

    // Method to simulate the physics world for a single time step
    virtual void stepSimulation(const float deltaTime, bool performCollisionDetection = false);
    virtual MathUtils::Vec3 detectCameraCollision(const MathUtils::Vec3& playerPosition, const MathUtils::Vec3& cameraPosition, const MathUtils::Vec3& cameraFront);

protected:
    PhysicsWorldImpl* m_pimpl;

};

#endif // PHYSICSWORLD_HPP
