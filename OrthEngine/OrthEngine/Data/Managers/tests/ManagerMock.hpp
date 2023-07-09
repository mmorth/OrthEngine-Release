#ifndef MANAGERMOCK_HPP
#define MANAGERMOCK_HPP

#include <gmock/gmock.h>

#include "Managers/RenderObjectManager.hpp"
#include "Managers/RenderObjectSceneCreator.hpp"

class RenderObjectSceneCreatorMock : public RenderObjectSceneCreator
{
public:
    MOCK_METHOD(std::vector<RenderObjectConfig>, createExperimentalScene, (), (override));
};

// TODO: Move this mock to ApplicationMock
class RenderObjectManagerMock : public RenderObjectManager
{
public:
    MOCK_METHOD(void, initialize, (const std::shared_ptr<FramebufferFactory> framebufferFactory, const std::shared_ptr<RenderObjectFactory> renderObjectFactory, const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<RenderObjectSceneCreator> renderObjectSceneCreator), ());
    MOCK_METHOD(void, loadScene, (const std::vector<RenderObjectConfig>&), (override));
    MOCK_METHOD(void, createRenderObject, (const RenderObjectConfig&), (override));
    MOCK_METHOD(void, updateFpsText, (const std::string&), (override));
    MOCK_METHOD(bool, updateRenderObject, (const RenderObjectProperties&, const RenderObjectConfig&), (override));
    MOCK_METHOD(void, removeObjectWithID, (const unsigned int), (override));
    MOCK_METHOD(void, createInstancedObject, (RenderObject* renderObjectPtr, (const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix), const float textureID), (override));
    MOCK_METHOD(bool, removeInstancedObjectWithID, (const unsigned int, const unsigned int), (override));
    MOCK_METHOD(void, updatePointLight, (RenderObject*), (override));
    MOCK_METHOD(void, updateFramebufferSize, (const int, const int), (override));
    MOCK_METHOD(TransformationMatrices, getTransformationMatrices, (const std::shared_ptr<Camera>&, const int, const int), (override));
    MOCK_METHOD(void, renderAll, (TransformationMatrices&, const RenderLoopConfigOptions&), (override));
    MOCK_METHOD(void, renderLoopSetup, (const SetupTeardownSettings&), (override));
    MOCK_METHOD(void, renderLoopCleanup, (const TransformationMatrices&, const SetupTeardownSettings&), (override));
};

#endif // MANAGERMOCK_HPP
