#ifndef MANAGERMOCK_HPP
#define MANAGERMOCK_HPP

#include <gmock/gmock.h>

#include "Managers/Collision/PhysicsManager.hpp"
#include "Managers/RenderObjectManager.hpp"
#include "Managers/RenderObjectSceneCreator.hpp"

class RenderObjectSceneCreatorMock : public RenderObjectSceneCreator
{
public:
    MOCK_METHOD(std::vector<ObjectConfig>, createExperimentalScene, (), (override));
};

class RenderObjectManagerMock : public RenderObjectManager 
{
public:
    MOCK_METHOD(void, initialize, (const std::shared_ptr<FramebufferFactory> framebufferFactory, const std::shared_ptr<RenderObjectFactory> renderObjectFactory, const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<RenderObjectSceneCreator> renderObjectSceneCreator), (override));
    MOCK_METHOD(RenderObject*, createRenderObject, (const unsigned int curID, const ObjectConfig& renderObjectConfig), (override));
    MOCK_METHOD(void, updateFpsText, (const std::string& newFpsString), (override));
    MOCK_METHOD(bool, updateRenderObject, (const RenderObjectProperties& renderObjectProperties, const ObjectConfig& renderObjectConfig), (override));
    MOCK_METHOD(void, removeObjectWithID, (const unsigned int objectID), (override));
    MOCK_METHOD(void, createInstancedObject, (RenderObject* renderObjectPtr, (const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix), const float textureID), (override));
    MOCK_METHOD(bool, removeInstancedObjectWithID, (const unsigned int renderObjectID, const unsigned int instancedID), (override));
    MOCK_METHOD(void, updatePointLight, (RenderObject* renderObjectPtr), (override));
    MOCK_METHOD(void, updateFramebufferSize, (const int screenWidth, const int screenHeight), (override));
    MOCK_METHOD(TransformationMatrices, getTransformationMatrices, (const std::shared_ptr<Camera>& camera, const int windowWidth, const int windowHeight), (override));
    MOCK_METHOD(void, renderAll, (TransformationMatrices& transformationMatrices, const RenderLoopConfigOptions& renderLoopConfigOptions), (override));
    MOCK_METHOD(void, renderLoopSetup, (const SetupTeardownSettings& setupTeardownSettings), (override));
    MOCK_METHOD(void, renderLoopCleanup, (const TransformationMatrices& transformationMatrices, const SetupTeardownSettings& setupTeardownSettings), (override));
    MOCK_METHOD(std::vector<unsigned char>, getFramebufferContent, (), (override));
};

class PhysicsManagerMock : public PhysicsManager 
{
public:
    MOCK_METHOD(void, initialize, (const std::shared_ptr<PhysicsWorld>& physicsWorld, const std::shared_ptr<RigidBodyFactory>& rigidBodyFactory), (override));
    MOCK_METHOD(void, cleanup, (), (override));
    MOCK_METHOD(void, createRigidBody, (const int id, ObjectConfig& renderObjectConfig, bool isPlayer), (override));
    MOCK_METHOD(void, removeRigidBodyWithID, (const unsigned int rigidBodyID, const unsigned int instanceID), (override));
    MOCK_METHOD(MathUtils::Vec3, getPlayerLocation, (), (override));
    MOCK_METHOD(void, updatePlayerLocation, (const MathUtils::Vec3& newPlayerLocation, const float timeToReach), (override));
    MOCK_METHOD(MathUtils::Vec3, detectCameraCollision, (const MathUtils::Vec3& playerPosition, const MathUtils::Vec3& cameraPosition, const MathUtils::Vec3& cameraFront), (override));
    MOCK_METHOD(std::optional<MathUtils::Vec3>, stepSimulationAndCheckCollision, (const float deltaTimeSec), (override));
};

#endif // MANAGERMOCK_HPP
