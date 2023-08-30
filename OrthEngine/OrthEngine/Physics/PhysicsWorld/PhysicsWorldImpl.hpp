#ifndef PHYSICSWORLDIMPL_HPP
#define PHYSICSWORLDIMPL_HPP

#include <algorithm>
#include <iostream>
#include <iterator>

#include <bullet/btBulletDynamicsCommon.h>

#include "BulletTypeConversions.hpp"
#include "ObjectStructs.hpp"

class PhysicsWorldImpl
{
public:
    PhysicsWorldImpl();
    ~PhysicsWorldImpl();

    // Methods for adding and removing collision shapes and rigid bodies to the world
    void addRigidBody(btRigidBody* body) { m_dynamicsWorld->addRigidBody(body); }
    void removeRigidBody(btRigidBody* body) { m_dynamicsWorld->removeRigidBody(body); }

    // Method to simulate the physics world for a single time step
    void stepSimulation(float deltaTime, bool performCollisionDetection = false);
    virtual MathUtils::Vec3 detectCameraCollision(const btVector3& playerPosition, const btVector3& cameraPosition, const btVector3& cameraFront);

protected:
    btDefaultCollisionConfiguration* m_collisionConfiguration;
    btCollisionDispatcher* m_dispatcher;
    btDbvtBroadphase* m_broadphase;
    btSequentialImpulseConstraintSolver* m_solver;
    btDiscreteDynamicsWorld* m_dynamicsWorld;

};

#endif // PHYSICSWORLDIMPL_HPP
