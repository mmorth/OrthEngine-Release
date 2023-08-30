#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Camera/Camera.hpp"
#include "Camera/tests/CameraMock.hpp"
#include "CameraFactory.hpp"

#include "FramebufferFactory.hpp"
#include "Framebuffer/Framebuffer.hpp"
#include "Framebuffer/tests/FramebufferMock.hpp"

#include "OpenGLFixture.hpp"

#include "PhysicsWorldMock.hpp"
#include "RigidBodyFactory.hpp"

#include "RasterizerFactory.hpp"
#include "Rasterizers/tests/RasterizerMock.hpp"
#include "RenderObjectFactory.hpp"
#include "RenderObjectMock.hpp"

#include "Shader/Shader.hpp"
#include "Shader/tests/ShaderMock.hpp"
#include "ShaderFactory.hpp"

#include "Texture/TextureFactory.hpp"
#include "TextureFactoryMock.hpp"

#include "VertexDataFactory.hpp"

#include "FactoryMock.hpp"

#include "Collision/PhysicsManager.hpp"
#include "RenderObjectManager.hpp"
#include "RenderObjectSceneCreator.hpp"
#include "ManagerMock.hpp"

namespace
{
    // ------------------------------------------------------------------------
    std::array<float, MathUtils::MAT4_SIZE> IDENTITY_MATRIX
    {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
};

// ===============================================================
// RenderObjectManagerTestable
// ===============================================================
class RenderObjectManagerTestable : public RenderObjectManager
{
public:
    RenderObjectManagerTestable()
        : RenderObjectManager() {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<FramebufferFactory> getFramebufferFactory() const { return m_framebufferFactory; }
    std::shared_ptr<RenderObjectFactory> getRenderObjectFactory() const { return m_renderObjectFactory; }
    std::shared_ptr<ShaderFactory> getShaderFactory() const { return m_shaderFactory; }
    std::shared_ptr<RenderObjectSceneCreator> getRenderObjectSceneCreator() const { return m_renderObjectSceneCreator; }

    std::unique_ptr<Framebuffer>& getFramebuffer() { return m_framebuffer; }
    std::unique_ptr<SkyboxObject>& getSkyboxObject() { return m_skyboxObject; }
    void setSkyboxObjectMock() {
        std::shared_ptr<Shader> shaderPtr = std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        std::shared_ptr<SkyboxRasterizer> skyboxRasterizerPtr = std::make_shared<SkyboxRasterizer>(VertexData({ 1.0f }, 1));

        m_skyboxObject = std::make_unique<SkyboxObjectMock>(shaderPtr, skyboxRasterizerPtr, 1);
    }

    const std::unordered_map<int, std::unique_ptr<RenderObject>>& getRenderObjects() const { return m_renderObjects; }
    const std::vector<unsigned int> getPlayerIDs() const { return m_playerIDs; }

    unsigned int getFPSTextID() const { return m_fpsTextID; }

    static unsigned int getReservedIDs() { return RESERVED_IDS; }

    // ===============================================================
    // Public Test Members
    // ===============================================================
    unsigned int instancedObjectID = 0;
};

// ===============================================================
// RenderObjectManagerTest
// ===============================================================
class RenderObjectManagerTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<RenderObjectFactoryMock> s_renderObjectFactoryMock;
    static std::shared_ptr<FramebufferFactoryMock> s_framebufferFactoryMock;
    static std::shared_ptr<ShaderFactoryMock> s_shaderFactoryMock;
    static std::shared_ptr<RenderObjectSceneCreatorMock> s_renderObjectSceneCreatorMock;
    static ::testing::StrictMock<RenderObjectManagerTestable>* s_renderObjectManagerTestable;
    static int curID;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_renderObjectFactoryMock = std::make_shared<RenderObjectFactoryMock>();
        s_framebufferFactoryMock = std::make_shared<FramebufferFactoryMock>();
        s_shaderFactoryMock = std::make_shared<ShaderFactoryMock>();
        s_renderObjectSceneCreatorMock = std::make_shared<RenderObjectSceneCreatorMock>();
        s_renderObjectManagerTestable = new ::testing::StrictMock<RenderObjectManagerTestable>();
        curID = 2;
    }

    static void TearDownTestCase()
    {
        s_renderObjectFactoryMock.reset();
        s_framebufferFactoryMock.reset();
        s_shaderFactoryMock.reset();
        s_renderObjectSceneCreatorMock.reset();
        delete s_renderObjectManagerTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<RenderObjectFactoryMock> RenderObjectManagerTest::s_renderObjectFactoryMock = nullptr;
std::shared_ptr<FramebufferFactoryMock> RenderObjectManagerTest::s_framebufferFactoryMock = nullptr;
std::shared_ptr<ShaderFactoryMock> RenderObjectManagerTest::s_shaderFactoryMock = nullptr;
std::shared_ptr<RenderObjectSceneCreatorMock> RenderObjectManagerTest::s_renderObjectSceneCreatorMock = nullptr;
::testing::StrictMock<RenderObjectManagerTestable>* RenderObjectManagerTest::s_renderObjectManagerTestable = nullptr;
int RenderObjectManagerTest::curID = 0;

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, RenderObjectManagerCorrectInitializesMemberVariables)
{
    // Verify FPS text is set
    EXPECT_CALL(*s_renderObjectFactoryMock, createRenderObject(::testing::_))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::unique_ptr<TextObjectMock>>(std::make_unique<TextObjectMock>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"), std::make_shared<TextRasterizer>(VertexData({ 1.0f }, 1)), TextProperties()))));

    // Set Framebuffer to Framebuffer mock
    EXPECT_CALL(*s_framebufferFactoryMock, getFramebuffer()).Times(1)
        .WillOnce(::testing::Return(std::make_unique<FramebufferMock>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"), VertexData({ 1.0f }, 1))));

    // FUT
    s_renderObjectManagerTestable->initialize(s_framebufferFactoryMock, s_renderObjectFactoryMock, s_shaderFactoryMock, s_renderObjectSceneCreatorMock);

    // Verify member variables are properly set
    EXPECT_TRUE(s_renderObjectManagerTestable->getSkyboxObject() == nullptr);
    EXPECT_EQ(1, s_renderObjectManagerTestable->getRenderObjects().size());
    EXPECT_EQ(1, s_renderObjectManagerTestable->getFPSTextID());
    EXPECT_EQ(0, s_renderObjectManagerTestable->getPlayerIDs().size());
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, CreateSkyboxRenderObjectCorrectlySetsSkyboxObject)
{
    size_t startNumRenderObjects = s_renderObjectManagerTestable->getRenderObjects().size();

    // Create three dummy render object configs to load
    ObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::SKYBOX, false };

    // Verify function calls
    EXPECT_CALL(*s_renderObjectFactoryMock, createRenderObject(::testing::_)).Times(1)
        .WillOnce(::testing::Return(std::optional<std::unique_ptr<SkyboxObjectMock>>(std::make_unique<SkyboxObjectMock>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"), std::make_shared<SkyboxRasterizer>(VertexData({ 1.0f }, 1)), 1))));

    // FUT
    s_renderObjectManagerTestable->createRenderObject(curID++, renderObjectConfig);

    EXPECT_TRUE(s_renderObjectManagerTestable->getSkyboxObject() != nullptr);
    EXPECT_EQ(startNumRenderObjects, s_renderObjectManagerTestable->getRenderObjects().size());
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, CreateInstancedObjectCorrectlyAddsInstancedObject)
{
    size_t startNumRenderObjects = s_renderObjectManagerTestable->getRenderObjects().size();
    s_renderObjectManagerTestable->instancedObjectID = curID;

    // Create three dummy render object configs to load
    ObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_CUBE, false };
    renderObjectConfig.instancedObjectProperties.modelMat = { {1.0f}, {2.0f} };
    renderObjectConfig.instancedObjectProperties.textureIDs = { 1.0f, 2.0f };

    // Verify function calls
    std::optional<std::unique_ptr<InstancedObjectMock>> renderObjectMockOpt = std::make_optional<std::unique_ptr<InstancedObjectMock>>(
        std::make_unique<InstancedObjectMock>(
            std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"),
            std::make_shared<InstancedRasterizer>(
                VertexData({ 1.0f }, 1),
                std::vector<std::array<float, MathUtils::MAT4_SIZE>>{},
                std::vector<float>{}
    )
            )
        );

    InstancedObjectMock* instMockPtr = renderObjectMockOpt->get();
    testing::Mock::AllowLeak(instMockPtr);

    EXPECT_CALL(*s_renderObjectFactoryMock, createRenderObject(::testing::_)).Times(1)
        .WillOnce(::testing::Return(::testing::ByMove(std::move(renderObjectMockOpt))));
    EXPECT_CALL(*instMockPtr, addInstancedObject(::testing::_, ::testing::_)).Times(2);

    // FUT
    s_renderObjectManagerTestable->createRenderObject(curID++, renderObjectConfig);

    EXPECT_EQ(startNumRenderObjects + 1, s_renderObjectManagerTestable->getRenderObjects().size());
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, CreatePointLightProperlyUpdatesLightProperties)
{
    size_t startNumRenderObjects = s_renderObjectManagerTestable->getRenderObjects().size();

    // Create three dummy render object configs to load
    ObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_POINT_LIGHT, false };
    renderObjectConfig.objectMaterialNames = { 32.0f, "Instanced_Texture_Array", "container2_specular.PNG" };
    renderObjectConfig.phongLightProperties = { {0.5f, 0.5f, 0.5f}, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } };
    renderObjectConfig.attenuationParams = { 1.0f, 0.09f, 0.032f };

    // Verify function calls
    std::optional<std::unique_ptr<PointLightMock>> renderObjectMockOpt = std::make_optional<std::unique_ptr<PointLightMock>>(
        std::make_unique<PointLightMock>(
            std::vector<std::shared_ptr<Shader>>{ std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom") },
            std::make_shared<InstancedRasterizer>(
                VertexData({ 1.0f }, 1),
                std::vector<std::array<float, MathUtils::MAT4_SIZE>>{},
                std::vector<float>{}
    ),
            ObjectMaterialProperties(), PhongLightingParams(), AttenuationParams()
        )
        );

    PointLightMock* instMockPtr = renderObjectMockOpt->get();
    testing::Mock::AllowLeak(instMockPtr);

    EXPECT_CALL(*s_renderObjectFactoryMock, createRenderObject(::testing::_)).Times(1)
        .WillOnce(::testing::Return(::testing::ByMove(std::move(renderObjectMockOpt))));
    EXPECT_CALL(*instMockPtr, updateLightProperties()).Times(1);

    // FUT
    s_renderObjectManagerTestable->createRenderObject(curID++, renderObjectConfig);

    EXPECT_EQ(startNumRenderObjects + 1, s_renderObjectManagerTestable->getRenderObjects().size());
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, CreatePlayerObjectCorrectlyTracksPlayerObjectID)
{
    size_t startNumRenderObjects = s_renderObjectManagerTestable->getRenderObjects().size();
    size_t startPlayerIDSize = s_renderObjectManagerTestable->getPlayerIDs().size();

    // Create three dummy render object configs to load
    ObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_CUBE, true };
    renderObjectConfig.objectLocationOrientation = { MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
    renderObjectConfig.objectMaterialNames = { 32.0f, "container2_resized.PNG", "container2_specular.PNG" };

    // Verify function calls
    std::optional<std::unique_ptr<NonInstancedObjectMock>> renderObjectMockOpt = std::make_optional<std::unique_ptr<NonInstancedObjectMock>>(
        std::make_unique<NonInstancedObjectMock>(
            std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"),
            std::make_shared<NonInstancedRasterizer>(
                VertexData({ 1.0f }, 1)
                ),
            ObjectLocationOrientation()
            )
        );

    EXPECT_CALL(*s_renderObjectFactoryMock, createRenderObject(::testing::_)).Times(1)
        .WillOnce(::testing::Return(::testing::ByMove(std::move(renderObjectMockOpt))));

    // FUT
    s_renderObjectManagerTestable->createRenderObject(curID++, renderObjectConfig);

    EXPECT_EQ(startNumRenderObjects + 1, s_renderObjectManagerTestable->getRenderObjects().size());
    EXPECT_EQ(startPlayerIDSize + 1, s_renderObjectManagerTestable->getPlayerIDs().size());
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, UpdateFPSTextCorrectlyUpdatesTextProperties)
{
    auto it = s_renderObjectManagerTestable->getRenderObjects().find(s_renderObjectManagerTestable->getFPSTextID());
    if (it != s_renderObjectManagerTestable->getRenderObjects().end())
    {
        auto fpsObj = dynamic_cast<TextObjectMock*>(it->second.get());
        testing::Mock::AllowLeak(fpsObj);
        EXPECT_CALL(*fpsObj, updateTextProperties(::testing::_)).Times(1);

        // FUT
        s_renderObjectManagerTestable->updateFpsText("New Test FPS Text");
    }
    else
    {
        EXPECT_TRUE(false) << "FPS Text Object is not found";
    }
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, RemoveInstancedObject)
{
    auto it = s_renderObjectManagerTestable->getRenderObjects().find(s_renderObjectManagerTestable->instancedObjectID);
    if (it != s_renderObjectManagerTestable->getRenderObjects().end())
    {
        unsigned int instanceToRemove = 0;
        auto instancedObj = dynamic_cast<InstancedObjectMock*>(it->second.get());
        testing::Mock::AllowLeak(instancedObj);
        EXPECT_CALL(*instancedObj, removeInstancedObject(::testing::Eq(instanceToRemove))).Times(1);

        // FUT
        s_renderObjectManagerTestable->removeInstancedObjectWithID(s_renderObjectManagerTestable->instancedObjectID, instanceToRemove);
    }
    else
    {
        EXPECT_TRUE(false) << "Instanced Object is not found";
    }
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, RemoveObject)
{
    size_t startNumRenderObjects = s_renderObjectManagerTestable->getRenderObjects().size();
    unsigned int objectIndexToRemove = s_renderObjectManagerTestable->getReservedIDs() + 1;

    // FUT
    s_renderObjectManagerTestable->removeObjectWithID(objectIndexToRemove);

    auto it = s_renderObjectManagerTestable->getRenderObjects().find(objectIndexToRemove);
    if (it == s_renderObjectManagerTestable->getRenderObjects().end())
    {
        EXPECT_EQ(startNumRenderObjects - 1, s_renderObjectManagerTestable->getRenderObjects().size());
    }
    else
    {
        EXPECT_TRUE(false) << "Instanced Object is not found";
    }
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, UpdateFramebufferSize)
{
    int expectedScreenWidth = 800; int expectedScreenHeight = 600; bool expectedMsaaEnabled = false;
    FramebufferMock& framebufferMockRef = dynamic_cast<FramebufferMock&>(*s_renderObjectManagerTestable->getFramebuffer());
    testing::Mock::AllowLeak(&framebufferMockRef);
    EXPECT_CALL(framebufferMockRef, updateFramebufferSize(::testing::Eq(expectedScreenWidth), ::testing::Eq(expectedScreenHeight), ::testing::Eq(expectedMsaaEnabled))).Times(1);
    EXPECT_CALL(framebufferMockRef, updateRenderbufferSize(::testing::Eq(expectedScreenWidth), ::testing::Eq(expectedScreenHeight))).Times(1);

    // FUT
    s_renderObjectManagerTestable->updateFramebufferSize(expectedScreenWidth, expectedScreenHeight);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, GetTransformationMatrices)
{
    int expectedScreenWidth = 800; int expectedScreenHeight = 600;
    std::shared_ptr<CameraMock> cameraMockPtr = std::make_shared<CameraMock>();

    EXPECT_CALL(*cameraMockPtr, GetZoom()).Times(1);
    EXPECT_CALL(*cameraMockPtr, GetViewMatrix()).Times(1);
    EXPECT_CALL(*cameraMockPtr, GetFrontVector()).Times(1);

    // FUT
    s_renderObjectManagerTestable->getTransformationMatrices(cameraMockPtr, expectedScreenWidth, expectedScreenHeight);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, RenderAllNoSetupAndTeardown)
{
    RenderLoopConfigOptions renderLoopConfigOptions = { {}, {}, {false, false, false, false} };

    // Verify rendering SkyboxObject first
    s_renderObjectManagerTestable->setSkyboxObjectMock();
    auto skyboxObjMock = dynamic_cast<SkyboxObjectMock*>(s_renderObjectManagerTestable->getSkyboxObject().get());
    testing::Mock::AllowLeak(skyboxObjMock);

    EXPECT_CALL(*skyboxObjMock, render(::testing::_, ::testing::_)).Times(1);

    // Verify all RenderObjects are rendered
    for (unsigned int i = 0; i < curID; i++)
    {
        auto it = s_renderObjectManagerTestable->getRenderObjects().find(i);
        if (it != s_renderObjectManagerTestable->getRenderObjects().end())
        {
            if (auto renderObjectMock = dynamic_cast<TextObjectMock*>(it->second.get()))
            {
                testing::Mock::AllowLeak(renderObjectMock);
                EXPECT_CALL(*renderObjectMock, render(::testing::_, ::testing::_)).Times(1);
            }

            if (auto renderObjectMock = dynamic_cast<PointLightMock*>(it->second.get()))
            {
                testing::Mock::AllowLeak(renderObjectMock);
                EXPECT_CALL(*renderObjectMock, render(::testing::_, ::testing::_)).Times(1);
            }

            if (auto renderObjectMock = dynamic_cast<NonInstancedObjectMock*>(it->second.get()))
            {
                testing::Mock::AllowLeak(renderObjectMock);
                EXPECT_CALL(*renderObjectMock, render(::testing::_, ::testing::_)).Times(1);
                EXPECT_CALL(*renderObjectMock, updateLocation(::testing::_)).Times(1);
            }
        }
    }

    // FUT 
    s_renderObjectManagerTestable->renderAll(TransformationMatrices(), RenderLoopConfigOptions());
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectManagerTest, RenderAllWithSetupAndTeardown)
{
    RenderLoopConfigOptions renderLoopConfigOptions = { {}, {}, {true, true, true, false} };
    renderLoopConfigOptions.setupTeardownSettings.framebufferEnabled = true;
    renderLoopConfigOptions.setupTeardownSettings.msaaEnabled = true;
    renderLoopConfigOptions.setupTeardownSettings.drawNormals = true;

    // Verify rendering SkyboxObject first
    s_renderObjectManagerTestable->setSkyboxObjectMock();
    auto skyboxObjMock = dynamic_cast<SkyboxObjectMock*>(s_renderObjectManagerTestable->getSkyboxObject().get());
    testing::Mock::AllowLeak(skyboxObjMock);
    EXPECT_CALL(*skyboxObjMock, render(::testing::_, ::testing::_)).Times(1);

    // Verify Framebuffer setup and teardown call
    FramebufferMock& framebufferMockRef = dynamic_cast<FramebufferMock&>(*s_renderObjectManagerTestable->getFramebuffer());
    testing::Mock::AllowLeak(&framebufferMockRef);
    EXPECT_CALL(framebufferMockRef, bindFramebuffer(::testing::_)).Times(2);
    EXPECT_CALL(framebufferMockRef, blitMSAA()).Times(1);
    EXPECT_CALL(framebufferMockRef, bindAndDrawQuadVAO()).Times(1);
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Normal"))).Times(1)
        .WillOnce(::testing::Return(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom")));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_InstancedNormal"))).Times(1)
        .WillOnce(::testing::Return(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom")));

    // Verify all RenderObjects are rendered
    for (unsigned int i = 0; i < curID; i++)
    {
        auto it = s_renderObjectManagerTestable->getRenderObjects().find(i);
        if (it != s_renderObjectManagerTestable->getRenderObjects().end())
        {
            if (auto renderObjectMock = dynamic_cast<TextObjectMock*>(it->second.get()))
            {
                testing::Mock::AllowLeak(renderObjectMock);
                EXPECT_CALL(*renderObjectMock, render(::testing::_, ::testing::_)).Times(1);
            }

            if (auto renderObjectMock = dynamic_cast<PointLightMock*>(it->second.get()))
            {
                testing::Mock::AllowLeak(renderObjectMock);
                EXPECT_CALL(*renderObjectMock, render(::testing::_, ::testing::_)).Times(1);
            }

            if (auto renderObjectMock = dynamic_cast<NonInstancedObjectMock*>(it->second.get()))
            {
                testing::Mock::AllowLeak(renderObjectMock);
                EXPECT_CALL(*renderObjectMock, render(::testing::_, ::testing::_)).Times(1);
                EXPECT_CALL(*renderObjectMock, updateLocation(::testing::_)).Times(1);
                EXPECT_CALL(*renderObjectMock, drawNormals(::testing::_, ::testing::_, ::testing::_)).Times(1);
            }
        }
    }

    // FUT 
    s_renderObjectManagerTestable->renderAll(TransformationMatrices(), renderLoopConfigOptions);
}


// ===============================================================
// PhysicsManagerTestable
// ===============================================================
class PhysicsManagerTestable : public PhysicsManager
{
public:
    PhysicsManagerTestable()
        : PhysicsManager() {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    const std::shared_ptr<PhysicsWorld> getPhysicsWorld() const { return m_physicsWorld; }
    const std::unordered_map<int, std::vector<std::unique_ptr<RigidBody>>>& getRigidBodies() const { return m_rigidBodies; }
    const int getPlayerID() const { return m_playerID; }
};

// ===============================================================
// PhysicsManagerTest
// ===============================================================
class PhysicsManagerTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<PhysicsWorldMock> s_physicsWorldMock;
    static std::shared_ptr<RigidBodyFactoryMock> s_rigidBodyFactoryMock;
    static ::testing::StrictMock<PhysicsManagerTestable>* s_physicsManagerTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_physicsWorldMock = std::make_shared<PhysicsWorldMock>();
        s_rigidBodyFactoryMock = std::make_shared<RigidBodyFactoryMock>();
        s_physicsManagerTestable = new ::testing::StrictMock<PhysicsManagerTestable>();

        s_physicsManagerTestable->initialize(s_physicsWorldMock, s_rigidBodyFactoryMock);
    }

    static void TearDownTestCase()
    {
        s_physicsWorldMock.reset();
        s_rigidBodyFactoryMock.reset();
        delete s_physicsManagerTestable;
        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<PhysicsWorldMock> PhysicsManagerTest::s_physicsWorldMock = nullptr;
std::shared_ptr<RigidBodyFactoryMock> PhysicsManagerTest::s_rigidBodyFactoryMock = nullptr;
::testing::StrictMock<PhysicsManagerTestable>* PhysicsManagerTest::s_physicsManagerTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, CreateInstancedRigidBodyCorrectlyCreatesInstanced)
{
    ObjectConfig instancedCubeObjectConfig;
    instancedCubeObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_CUBE, false, true };
    instancedCubeObjectConfig.instancedObjectProperties.modelMat.push_back(IDENTITY_MATRIX);
    instancedCubeObjectConfig.instancedObjectProperties.modelMat.push_back(IDENTITY_MATRIX);
    instancedCubeObjectConfig.instancedObjectProperties.textureIDs.push_back(1.0f);
    instancedCubeObjectConfig.instancedObjectProperties.textureIDs.push_back(1.0f);
    int curID = s_physicsManagerTestable->getRigidBodies().size();

    EXPECT_CALL(*s_rigidBodyFactoryMock, createRigidBody(::testing::_)).Times(0);
    EXPECT_CALL(*s_physicsWorldMock, addRigidBody(::testing::_)).Times(2);

    // FUT
    s_physicsManagerTestable->createRigidBody(curID, instancedCubeObjectConfig, false);
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, CreateNonInstancedRigidBodyCorrectlyCreatesNonInstanced)
{
    ObjectConfig objectConfig;
    objectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_CUBE, false, true };
    int curID = s_physicsManagerTestable->getRigidBodies().size();

    EXPECT_CALL(*s_rigidBodyFactoryMock, createRigidBody(::testing::_)).Times(0);
    EXPECT_CALL(*s_physicsWorldMock, addRigidBody(::testing::_)).Times(1);

    // FUT
    s_physicsManagerTestable->createRigidBody(curID, objectConfig, false);

    EXPECT_EQ(s_physicsManagerTestable->getRigidBodies().size(), curID + 1);
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, CreatePlayerRigidBodyCorrectlyStoresIDAsPlayer)
{
    ObjectConfig objectConfig;
    int curID = s_physicsManagerTestable->getRigidBodies().size();
    
    // FUT
    s_physicsManagerTestable->createRigidBody(curID, objectConfig, true);

    EXPECT_EQ(s_physicsManagerTestable->getPlayerID(), curID);
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, RemoveRigidBodyCorrectlyRemovesTheRigidBodyAtIndex)
{
    int previousRigidObjectSize = s_physicsManagerTestable->getRigidBodies().size();

    s_physicsManagerTestable->removeRigidBodyWithID(1, 0);

    EXPECT_EQ(s_physicsManagerTestable->getRigidBodies().size(), previousRigidObjectSize);
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, UpdateAndGetPlayerLocationCorrectlyHandlesPlayerLocation)
{
    MathUtils::Vec3 newPlayerPosition(1.0f, 2.0f, 3.0f);
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, CleanupProperlyRemovesAllRigidBodies)
{
    EXPECT_CALL(*s_physicsWorldMock, removeRigidBody(::testing::_)).Times(3);

    // FUT
    s_physicsManagerTestable->cleanup();
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, DetectCameraCollisionCorrectlyCallsPhysicsWorldDetectCameraCollision)
{
    EXPECT_CALL(*s_physicsWorldMock, detectCameraCollision(::testing::_, ::testing::_, ::testing::_)).Times(1);

    // FUT
    s_physicsManagerTestable->detectCameraCollision(MathUtils::Vec3(0.0f, 0.0f, 0.0f), MathUtils::Vec3(0.0f, 0.0f, 0.0f), MathUtils::Vec3(0.0f, 0.0f, 0.0f));
}

// ------------------------------------------------------------------------
TEST_F(PhysicsManagerTest, StepSimulationAndCheckSimulationPerformsBothOperationsOnPhysicsWorld)
{
    EXPECT_CALL(*s_physicsWorldMock, stepSimulation(::testing::_, ::testing::_)).Times(1);

    // FUT
    s_physicsManagerTestable->stepSimulationAndCheckCollision(1.0f);
}
