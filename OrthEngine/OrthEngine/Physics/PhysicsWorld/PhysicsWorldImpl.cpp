#include "PhysicsWorldImpl.hpp"

// ------------------------------------------------------------------------
PhysicsWorldImpl::PhysicsWorldImpl()
{
    // Initialize Bullet Physics components
    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    m_broadphase = new btDbvtBroadphase();
    m_solver = new btSequentialImpulseConstraintSolver();
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
    //m_dynamicsWorld->setGravity(btVector3(0, -9.81, 0));
    m_dynamicsWorld->setGravity(btVector3(0, 0, 0));
}

// ------------------------------------------------------------------------
PhysicsWorldImpl::~PhysicsWorldImpl()
{
    // Clean up Bullet Physics components
    delete m_dynamicsWorld;
    delete m_solver;
    delete m_broadphase;
    delete m_dispatcher;
    delete m_collisionConfiguration;
}

// ------------------------------------------------------------------------
void PhysicsWorldImpl::stepSimulation(float deltaTime, bool performCollisionDetection)
{ 
    if (performCollisionDetection)
        m_dynamicsWorld->performDiscreteCollisionDetection();
    
    // TODO: Monitor the maxSubstep performance if you have issues
    m_dynamicsWorld->stepSimulation(deltaTime, 10.0f); 
}

// ------------------------------------------------------------------------
MathUtils::Vec3 PhysicsWorldImpl::detectCameraCollision(const btVector3& playerPosition, const btVector3& cameraPosition, const btVector3& cameraFront)
{
    // Perform raycasting
    btCollisionWorld::ClosestRayResultCallback rayCallback(playerPosition, cameraPosition);
    rayCallback.m_collisionFilterGroup = btBroadphaseProxy::DefaultFilter;
    rayCallback.m_collisionFilterMask = btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::DefaultFilter;

    m_dynamicsWorld->rayTest(playerPosition, cameraPosition, rayCallback);
    btVector3 newCameraPosition = cameraPosition;

    // If ray hits an object
    if (rayCallback.hasHit()) 
        newCameraPosition = rayCallback.m_hitPointWorld;

    return BtConversions::btToMathVec3(newCameraPosition);
}
