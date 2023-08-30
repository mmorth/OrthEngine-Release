#ifndef PHYSICSMANAGER_HPP
#define PHYSICSMANAGER_HPP

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>

#include "RenderObjects/RenderObject.hpp"
#include "RenderObjects/InstancedObject.hpp"
#include "RenderObjects/NonInstancedObject.hpp"

#include "Physics/PhysicsWorld/PhysicsWorld.hpp"
#include "Physics/RigidBody/RigidBody.hpp"
#include "RigidBodyFactory.hpp"
#include "ObjectStructs.hpp"

class PhysicsManager
{
public:
    PhysicsManager();
    ~PhysicsManager();

    virtual void initialize(const std::shared_ptr<PhysicsWorld>& physicsWorld = nullptr, const std::shared_ptr<RigidBodyFactory>& rigidBodyFactory = nullptr);
    virtual void cleanup();

    virtual void createRigidBody(const int id, ObjectConfig& renderObjectConfig, bool isPlayer);
    virtual void removeRigidBodyWithID(const unsigned int rigidBodyID, const unsigned int instanceID);

    virtual MathUtils::Vec3 getPlayerLocation();
    virtual void updatePlayerLocation(const MathUtils::Vec3& newPlayerLocation, const float timeToReach);
    virtual MathUtils::Vec3 detectCameraCollision(const MathUtils::Vec3& playerPosition, const MathUtils::Vec3& cameraPosition, const MathUtils::Vec3& cameraFront);
    virtual std::optional<MathUtils::Vec3> stepSimulationAndCheckCollision(const float deltaTimeSec);

protected:
    std::shared_ptr<PhysicsWorld> m_physicsWorld;
    std::shared_ptr<RigidBodyFactory> m_rigidBodyFactory;
    std::unordered_map<int, std::vector<std::unique_ptr<RigidBody>>> m_rigidBodies;

    int m_playerID;

};

#endif // PHYSICSMANAGER_HPP