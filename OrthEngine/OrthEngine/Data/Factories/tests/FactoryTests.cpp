#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Camera/Camera.hpp"
#include "Camera/tests/CameraMock.hpp"
#include "CameraFactory.hpp"

#include "FramebufferFactory.hpp"
#include "Framebuffer/Framebuffer.hpp"
#include "Framebuffer/tests/FramebufferMock.hpp"

#include "OpenGLFixture.hpp"

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


// ===============================================================
// VertexDataFactoryTest
// ===============================================================
class VertexDataFactoryTest : public ::testing::Test
{
protected:
    void SetUp() override {}
    void TearDown() override {}

};

// ------------------------------------------------------------------------
TEST_F(VertexDataFactoryTest, CreateVertexDataFactoryCorrectlyCreatesDefaultVertexData)
{
    VertexDataFactory vertexDataFactory = VertexDataFactory::getInstance(); // FUT

    // Verify Default Cube properties
    std::optional<VertexData> cubeVertexDataOpt = vertexDataFactory.getVertexData("Default_Cube");
    EXPECT_TRUE(cubeVertexDataOpt.has_value()); VertexData cubeVertexData = cubeVertexDataOpt.value();
    constexpr unsigned int expectedCubeAttributesPerVertex = 8;  EXPECT_EQ(expectedCubeAttributesPerVertex, cubeVertexData.attributesPerVertex);
    std::vector<float> expectedCubeVertexAttributes = 
    {
        // positions          // normals           // texture coords
        // back face
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
        // front face
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        // left face
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
        // right face
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
         // bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
        // top face
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
    };
    EXPECT_EQ(expectedCubeVertexAttributes, cubeVertexData.vertexAttributes);

    // Verify Default Cube properties
    std::optional<VertexData> planeVertexDataOpt = vertexDataFactory.getVertexData("Default_Plane");
    EXPECT_TRUE(planeVertexDataOpt.has_value()); VertexData planeVertexData = planeVertexDataOpt.value();
    constexpr unsigned int expectedPlaneAttributesPerVertex = 8;  EXPECT_EQ(expectedPlaneAttributesPerVertex, planeVertexData.attributesPerVertex);
    std::vector<float> expectedPlaneVertexAttributes = 
    {
        // positions         // normals           // texture Coords
        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
        1.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
        0.0f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,

        0.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        1.0f,  0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
        1.0f, -0.5f,  0.0f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f
    };
    EXPECT_EQ(expectedPlaneVertexAttributes, planeVertexData.vertexAttributes);

    std::optional<VertexData> quadVertexDataOpt = vertexDataFactory.getVertexData("Default_Quad");
    EXPECT_TRUE(quadVertexDataOpt.has_value()); VertexData quadVertexData = quadVertexDataOpt.value();
    constexpr unsigned int expectedQuadAttributesPerVertex = 4;  EXPECT_EQ(expectedQuadAttributesPerVertex, quadVertexData.attributesPerVertex);
    std::vector<float> expectedQuadVertexAttributes = 
    {
        // positions   // texCoords
        -1.0f,  1.0f,  0.0f, 1.0f,
        -1.0f, -1.0f,  0.0f, 0.0f,
         1.0f, -1.0f,  1.0f, 0.0f,

        -1.0f,  1.0f,  0.0f, 1.0f,
         1.0f, -1.0f,  1.0f, 0.0f,
         1.0f,  1.0f,  1.0f, 1.0f
    };
    EXPECT_EQ(expectedQuadVertexAttributes, quadVertexData.vertexAttributes);

    std::optional<VertexData> skyboxVertexDataOpt = vertexDataFactory.getVertexData("Default_Skybox");
    EXPECT_TRUE(skyboxVertexDataOpt.has_value()); VertexData skyboxVertexData = skyboxVertexDataOpt.value();
    constexpr unsigned int expectedSkyboxAttributesPerVertex = 3;  EXPECT_EQ(expectedSkyboxAttributesPerVertex, skyboxVertexData.attributesPerVertex);
    std::vector<float> expectedSkyboxVertexAttributes = 
    {
        // positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };
    EXPECT_EQ(expectedSkyboxVertexAttributes, skyboxVertexData.vertexAttributes);
}

// ------------------------------------------------------------------------
TEST_F(VertexDataFactoryTest, AddVertexDataCorrectlyAddsNewVertexDataToMap)
{
    VertexDataFactory vertexDataFactory = VertexDataFactory::getInstance();

    std::string expectedVertexDataKey = "Test Vertex";
    VertexData expectedVertexData{ {1.0f, 2.0f, 3.0f}, 1 };
    vertexDataFactory.addVertexData(expectedVertexDataKey, expectedVertexData); // FUT

    std::optional<VertexData> vertexDataOpt = vertexDataFactory.getVertexData(expectedVertexDataKey);
    EXPECT_TRUE(vertexDataOpt.has_value()); VertexData vertexData = vertexDataOpt.value();
    EXPECT_EQ(expectedVertexData.attributesPerVertex, vertexData.attributesPerVertex);
    EXPECT_EQ(expectedVertexData.vertexAttributes, vertexData.vertexAttributes);
}

// ------------------------------------------------------------------------
TEST_F(VertexDataFactoryTest, VertexDataFactoryReturnsTheSameInstancedEachTime)
{
    EXPECT_EQ(&VertexDataFactory::getInstance(), &VertexDataFactory::getInstance());
}


// ===============================================================
// ShaderFactoryTest
// ===============================================================
class ShaderFactoryTest : public OpenGLTestFixture
{
public:
    void verifyShaderFactoryGet(std::string shaderKey)
    {
        std::optional<std::shared_ptr<Shader>> shaderOpt = ShaderFactory::getInstance().getShader(shaderKey);
        EXPECT_TRUE(shaderOpt.has_value()); std::shared_ptr<Shader> shader = shaderOpt.value();
        EXPECT_TRUE(shader->getID() > 0); shader->use();
    }

protected:
    void SetUp() override {}
    void TearDown() override {}

};

// ------------------------------------------------------------------------
TEST_F(ShaderFactoryTest, CreateShaderFactoryCorrectlyCreatesDefaultShaders)
{
    // Verify all default shaders set during constructor
    verifyShaderFactoryGet("Default_Object");
    verifyShaderFactoryGet("Default_InstancedObject");
    verifyShaderFactoryGet("Default_Light");
    verifyShaderFactoryGet("Default_Skybox");
    verifyShaderFactoryGet("Default_Text");
    verifyShaderFactoryGet("Default_Framebuffer");
    verifyShaderFactoryGet("Default_Outline");
    verifyShaderFactoryGet("Default_Normal");
    verifyShaderFactoryGet("Default_InstancedNormal");
}

// ------------------------------------------------------------------------
TEST_F(ShaderFactoryTest, AddShaderCorrectlyAddsNewShadersToMap)
{
    std::string expectedShaderKey = "Test Shader";
    ShaderFileNames shaderFileNames = { "ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom" };

    ShaderFactory::getInstance().addShader(expectedShaderKey, shaderFileNames); // FUT

    verifyShaderFactoryGet(expectedShaderKey);
}

// ------------------------------------------------------------------------
TEST_F(ShaderFactoryTest, ShaderFactoryReturnsTheSameInstancedEachTime)
{
    EXPECT_EQ(&ShaderFactory::getInstance(), &ShaderFactory::getInstance());
}


// ===============================================================
// CameraFactoryTest
// ===============================================================
class CameraFactoryTest : public OpenGLTestFixture
{
protected:
    void SetUp() override {}
    void TearDown() override {}

};

// ------------------------------------------------------------------------
TEST_F(CameraFactoryTest, GetCamerasCorrectlyReturnsCorrectCameraTypeBasedOnParameter)
{
    // Verify first and third person cameras are returned when requested
    CameraFactory cameraFactory = CameraFactory::getInstance();
    MathUtils::Vec3 expectedCamPosition(1.0f, 2.0f, 3.0f);

    // Verify first person camera
    std::unique_ptr<Camera> firstPersonCameraPtr = cameraFactory.getCamera(CameraTypes::FIRST_PERSON, expectedCamPosition);
    auto firstPersonCamera = dynamic_cast<FirstPersonCamera*>(firstPersonCameraPtr.get()); EXPECT_TRUE(firstPersonCamera != nullptr);
    EXPECT_EQ(expectedCamPosition, firstPersonCamera->GetPosition());

    // Verify third person camera
    std::unique_ptr<Camera> thirdPersonCameraPtr = cameraFactory.getCamera(CameraTypes::THIRD_PERSON, expectedCamPosition);
    auto thirdPersonCamera = dynamic_cast<ThirdPersonCamera*>(thirdPersonCameraPtr.get()); EXPECT_TRUE(thirdPersonCamera != nullptr);
    EXPECT_NE(expectedCamPosition, thirdPersonCamera->GetPosition());

    // TODO: Add a front vector to the camera factory
}

// ------------------------------------------------------------------------
TEST_F(CameraFactoryTest, CameraFactoryReturnsTheSameInstancedEachTime)
{
    EXPECT_EQ(&CameraFactory::getInstance(), &CameraFactory::getInstance());
}


// ===============================================================
// FramebufferFactoryTest
// ===============================================================
class FramebufferFactoryTest : public OpenGLTestFixture
{
protected:
    void SetUp() override {}
    void TearDown() override {}

};

// ------------------------------------------------------------------------
TEST_F(FramebufferFactoryTest, CreateFramebufferCorrectlyCreatesPlaneFramebuffer)
{
    std::shared_ptr<ShaderFactoryMock> shaderFactoryMock = std::make_shared<ShaderFactoryMock>();
    std::shared_ptr<VertexDataFactoryMock> vertexDataFactoryMock = std::make_shared<VertexDataFactoryMock>();
    FramebufferFactory::getInstance().initialize(shaderFactoryMock, vertexDataFactoryMock);

    EXPECT_CALL(*shaderFactoryMock, getShader(::testing::Eq("Default_Framebuffer")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*vertexDataFactoryMock, getVertexData(::testing::Eq("Default_Quad")))
        .Times(1)
        .WillOnce(::testing::Return(std::make_optional<VertexData>(std::vector<float>{1.0f}, 8)));

    std::unique_ptr<Framebuffer> framebuffer = FramebufferFactory::getInstance().getFramebuffer(); // FUT
}

// ------------------------------------------------------------------------
TEST_F(FramebufferFactoryTest, FramebufferFactoryReturnsTheSameInstancedEachTime)
{
    EXPECT_EQ(&FramebufferFactory::getInstance(), &FramebufferFactory::getInstance());
}


// ===============================================================
// RasterizerFactoryTest
// ===============================================================
class RasterizerFactoryTest : public OpenGLTestFixture
{
public:
    void verifyRasterizer(GeometryTypes rasterizerType)
    {
        std::optional<std::shared_ptr<Rasterizer>> rasterizerOpt = RasterizerFactory::getInstance().getRasterizer(rasterizerType);
        EXPECT_TRUE(rasterizerOpt.has_value()); std::shared_ptr<Rasterizer> rasterizer = rasterizerOpt.value();
        
        // verify expected number of vertex attributes
        switch (rasterizerType)
        {
            case GeometryTypes::INSTANCED_CUBE:
            case GeometryTypes::NONINSTANCED_CUBE:
            case GeometryTypes::QUAD:
            case GeometryTypes::INSTANCED_POINT_LIGHT:
                EXPECT_EQ(36, rasterizer->getNumVertices());
                break;

            case GeometryTypes::INSTANCED_PLANE:
            case GeometryTypes::NONINSTANCED_PLANE:
                EXPECT_EQ(12, rasterizer->getNumVertices());
                break;

            
            case GeometryTypes::SKYBOX:
                EXPECT_EQ(36, rasterizer->getNumVertices());
                break;

            case GeometryTypes::TEXT:
                EXPECT_EQ(6, rasterizer->getNumVertices());
                break;

            default:
                break;
        }

        // verify correct rasterizer type
        switch (rasterizerType)
        {
            case GeometryTypes::INSTANCED_CUBE:
            case GeometryTypes::INSTANCED_PLANE:
            case GeometryTypes::INSTANCED_POINT_LIGHT:
            {
                auto rasterizerClassType = dynamic_cast<InstancedRasterizer*>(rasterizer.get()); EXPECT_TRUE(rasterizerClassType != nullptr);
                break;
            }

            case GeometryTypes::NONINSTANCED_CUBE:
            case GeometryTypes::NONINSTANCED_PLANE:
            case GeometryTypes::QUAD:
            {
                auto rasterizerClassType = dynamic_cast<NonInstancedRasterizer*>(rasterizer.get()); EXPECT_TRUE(rasterizerClassType != nullptr);
                break;
            }

            case GeometryTypes::SKYBOX:
            {
                auto rasterizerClassType = dynamic_cast<SkyboxRasterizer*>(rasterizer.get()); EXPECT_TRUE(rasterizerClassType != nullptr);
                break;
            }

            case GeometryTypes::TEXT:
            {
                auto rasterizerClassType = dynamic_cast<TextRasterizer*>(rasterizer.get()); EXPECT_TRUE(rasterizerClassType != nullptr);
                break;
            }

            default:
                break;
        }
    }

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
    }

    static void TearDownTestCase()
    {
        OpenGLTestFixture::TearDownTestCase();
    }

};

// ------------------------------------------------------------------------
TEST_F(RasterizerFactoryTest, VerifyDefaultRasterizersSetCorrectly)
{
    verifyRasterizer(GeometryTypes::INSTANCED_CUBE);
    verifyRasterizer(GeometryTypes::NONINSTANCED_CUBE);
    verifyRasterizer(GeometryTypes::INSTANCED_PLANE);
    verifyRasterizer(GeometryTypes::NONINSTANCED_PLANE);
    verifyRasterizer(GeometryTypes::QUAD);
    verifyRasterizer(GeometryTypes::SKYBOX);
    verifyRasterizer(GeometryTypes::TEXT);
    verifyRasterizer(GeometryTypes::INSTANCED_POINT_LIGHT);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerFactoryTest, RasterizerUtilityDefaultsSetOnRasterizerConstructor)
{
    RasterizerFactory::getInstance().setRasterizerDefault();

    // verify depth set
    bool isDepthEnabled = glIsEnabled(GL_DEPTH_TEST); EXPECT_EQ(true, isDepthEnabled);

    // verify blend enable
    bool isBlendEnabled = glIsEnabled(GL_BLEND); EXPECT_EQ(true, isBlendEnabled);
    GLenum blendSrcFactor; glGetIntegerv(GL_BLEND_DST_ALPHA, reinterpret_cast<GLint*>(&blendSrcFactor)); EXPECT_EQ(blendSrcFactor, GL_ONE_MINUS_SRC_ALPHA);

    // verify culling set
    bool isSupersampleEnabled = glIsEnabled(GL_CULL_FACE); EXPECT_EQ(true, isSupersampleEnabled);
    GLenum currentCullFaceMode; glGetIntegerv(GL_CULL_FACE_MODE, reinterpret_cast<GLint*>(&currentCullFaceMode)); EXPECT_EQ(GL_BACK, currentCullFaceMode);
    GLenum currentFrontFaceOrientation; glGetIntegerv(GL_FRONT_FACE, reinterpret_cast<GLint*>(&currentFrontFaceOrientation)); EXPECT_EQ(GL_CCW, currentFrontFaceOrientation);

    // verify wireframe drawn
    GLenum currentFrontMode, currentBackMode;
    glGetIntegerv(GL_POLYGON_MODE, reinterpret_cast<GLint*>(&currentFrontMode));
    glGetIntegerv(GL_POLYGON_MODE, reinterpret_cast<GLint*>(&currentBackMode));
    EXPECT_EQ(GL_FILL, currentFrontMode); EXPECT_EQ(GL_FILL, currentBackMode);
}

// ------------------------------------------------------------------------
TEST_F(RasterizerFactoryTest, RasterizerFactoryReturnsTheSameInstancedEachTime)
{
    EXPECT_EQ(&RasterizerFactory::getInstance(), &RasterizerFactory::getInstance());
}


// ===============================================================
// RenderObjectFactoryTest
// ===============================================================
class RenderObjectFactoryTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<ShaderFactoryMock> s_shaderFactoryMock;
    static std::shared_ptr<RasterizerFactoryMock> s_rasterizerFactoryMock;
    static std::shared_ptr<TextureFactoryMock> s_textureFactoryMock;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_shaderFactoryMock = std::make_shared<ShaderFactoryMock>();
        s_rasterizerFactoryMock = std::make_shared<RasterizerFactoryMock>();
        s_textureFactoryMock = std::make_shared<TextureFactoryMock>();

        RenderObjectFactory::getInstance().initialize(s_shaderFactoryMock, s_rasterizerFactoryMock, s_textureFactoryMock);
    }

    static void TearDownTestCase()
    {
        s_shaderFactoryMock.reset();
        s_rasterizerFactoryMock.reset();
        s_textureFactoryMock.reset();

        OpenGLTestFixture::TearDownTestCase();
    }

    std::unique_ptr<RenderObject> verifyRenderObject(RenderObjectConfig renderObjectConfig)
    {
        std::optional<std::unique_ptr<RenderObject>> renderObjectOpt = RenderObjectFactory::getInstance().createRenderObject(renderObjectConfig);
        EXPECT_TRUE(renderObjectOpt.has_value()); 

        return std::move(renderObjectOpt.value());
    }

};

std::shared_ptr<ShaderFactoryMock> RenderObjectFactoryTest::s_shaderFactoryMock;
std::shared_ptr<RasterizerFactoryMock> RenderObjectFactoryTest::s_rasterizerFactoryMock;
std::shared_ptr<TextureFactoryMock> RenderObjectFactoryTest::s_textureFactoryMock;

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateInstancedCube)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_CUBE, false };
    renderObjectConfig.objectMaterialNames = { 32.0f, "Instanced_Texture_Array", "container2_specular.PNG" };

    // Expect Calls
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq(renderObjectConfig.objectMaterialNames.diffuseMap)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq(renderObjectConfig.objectMaterialNames.specularMap)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_InstancedObject")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_rasterizerFactoryMock, getRasterizer(::testing::Eq(renderObjectConfig.renderObjectProperties.geometryType)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<InstancedRasterizer>>(std::make_shared<InstancedRasterizer>(VertexData({}, 8), std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{}))));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<GeometricInstancedObject*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateNonInstancedCube)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_CUBE, false };
    renderObjectConfig.objectLocation = { MathUtils::Vec3{-1.0f, -1.0f, -1.0f}, MathUtils::Vec3{1.0f, 1.0f, 1.0f}, 0.0f };
    renderObjectConfig.objectMaterialNames = { 32.0f, "container2_resized.PNG", "container2_specular.PNG" };

    // Expect Calls
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq(renderObjectConfig.objectMaterialNames.diffuseMap)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq(renderObjectConfig.objectMaterialNames.specularMap)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Object")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_rasterizerFactoryMock, getRasterizer(::testing::Eq(renderObjectConfig.renderObjectProperties.geometryType)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<NonInstancedRasterizer>>(std::make_shared<NonInstancedRasterizer>(VertexData({}, 8)))));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<GeometricNonInstancedObject*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateSkybox)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::SKYBOX, false };

    // Expect Calls
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Skybox")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_rasterizerFactoryMock, getRasterizer(::testing::Eq(renderObjectConfig.renderObjectProperties.geometryType)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<SkyboxRasterizer>>(std::make_shared<SkyboxRasterizer>(VertexData({}, 8)))));
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq("Default_Skybox")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<SkyboxObject*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateText)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::TEXT, false };
    renderObjectConfig.textProperties = { "Sample Text", {50.0f, 50.0f}, 2.0f, {1.0f, 1.0f, 1.0f} };

    // Expect Calls
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Text")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_rasterizerFactoryMock, getRasterizer(::testing::Eq(renderObjectConfig.renderObjectProperties.geometryType)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<TextRasterizer>>(std::make_shared<TextRasterizer>(VertexData({}, 8)))));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<TextObject*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateInstancedPointLight)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_POINT_LIGHT, false };
    renderObjectConfig.objectMaterialNames = { 32.0f, "Instanced_Texture_Array", "container2_specular.PNG" };
    renderObjectConfig.phongLightProperties = { {0.5f, 0.5f, 0.5f}, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } };
    renderObjectConfig.attenuationParams = { 1.0f, 0.09f, 0.032f };

    // Expect Calls
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq(renderObjectConfig.objectMaterialNames.diffuseMap)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));
    EXPECT_CALL(*s_textureFactoryMock, getTextureID(::testing::Eq(renderObjectConfig.objectMaterialNames.specularMap)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<unsigned int>(1)));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_InstancedObject")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Object")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_rasterizerFactoryMock, getRasterizer(::testing::Eq(renderObjectConfig.renderObjectProperties.geometryType)))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<InstancedRasterizer>>(std::make_shared<InstancedRasterizer>(VertexData({}, 8), std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{}))));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<PointLight*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateInstancedSpotLight)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_SPOT_LIGHT, false };
    renderObjectConfig.lightProperties = { MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3{ 0.0f, 0.0f, 0.0f } };
    renderObjectConfig.phongLightProperties = { {0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } };
    renderObjectConfig.attenuationParams = { 1.0f, 0.09f, 0.032f };
    renderObjectConfig.spotlightCutoffParams = { 12.5f, 15.0f };

    // Expect Calls
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_InstancedObject")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Object")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<SpotLight*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateInstancedDirectionalLight)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::DIRECTIONAL_LIGHT, false };
    renderObjectConfig.lightProperties = { MathUtils::Vec3{ 0.0f, -1.0f, 0.0f }, MathUtils::Vec3{ 0.0f, -1.0f, 0.0f } };
    renderObjectConfig.phongLightProperties = { { 0.05f, 0.05f, 0.05f }, { 0.4f, 0.4f, 0.4f }, { 0.5f, 0.5f, 0.5f } };

    // Expect Calls
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_InstancedObject")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));
    EXPECT_CALL(*s_shaderFactoryMock, getShader(::testing::Eq("Default_Object")))
        .Times(1)
        .WillOnce(::testing::Return(std::optional<std::shared_ptr<Shader>>(std::make_shared<Shader>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom"))));

    // FUT call
    std::unique_ptr<RenderObject> instancedCubeObject = verifyRenderObject(renderObjectConfig);
    auto renderObjectClassType = dynamic_cast<DirectionalLight*>(instancedCubeObject.get()); EXPECT_TRUE(renderObjectClassType != nullptr);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreatePlane)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_PLANE, false };

    std::optional<std::unique_ptr<RenderObject>> renderObjectOpt = RenderObjectFactory::getInstance().createRenderObject(renderObjectConfig);
    EXPECT_EQ(std::nullopt, renderObjectOpt);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryCreateQuad)
{
    // Create RenderObject
    RenderObjectConfig renderObjectConfig;
    renderObjectConfig.renderObjectProperties = { GeometryTypes::QUAD, false };

    std::optional<std::unique_ptr<RenderObject>> renderObjectOpt = RenderObjectFactory::getInstance().createRenderObject(renderObjectConfig);
    EXPECT_EQ(std::nullopt, renderObjectOpt);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectFactoryTest, RenderObjectFactoryReturnsTheSameInstancedEachTime)
{
    EXPECT_EQ(&RenderObjectFactory::getInstance(), &RenderObjectFactory::getInstance());
}
