#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Rasterizers/tests/RasterizerMock.hpp"
#include "Shader/tests/ShaderMock.hpp"
#include "Transform/tests/TransformMock.hpp"
#include "ObjectStructs.hpp"
#include "OpenGLFixture.hpp"

#include "RenderObjects/RenderObject.hpp"
#include "RenderObjects/InstancedObject.hpp"
#include "RenderObjects/NonInstancedObject.hpp"

#include "RenderObjects/GeometricObjects/GeometricInstancedObject.hpp"
#include "RenderObjects/GeometricObjects/GeometricNonInstancedObject.hpp"

#include "RenderObjects/LightObjects/LightObject.hpp"
#include "RenderObjects/LightObjects/DirectionalLight.hpp"
#include "RenderObjects/LightObjects/LightObject.hpp"
#include "RenderObjects/LightObjects/PointLight.hpp"
#include "RenderObjects/LightObjects/SpotLight.hpp"

#include "RenderObjects/OtherRenders/SkyboxObject.hpp"
#include "RenderObjects/OtherRenders/TextObject.hpp"

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

    std::array<float, MathUtils::MAT4_SIZE> ZERO_MATRIX
    {
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0,
        0, 0, 0, 0
    };

    template <typename T>
    void verifyRenderObjectConstructorSetsShader(::testing::StrictMock<T>& renderObjectTestable)
    {
        EXPECT_TRUE(renderObjectTestable.getShader() != nullptr);
    }

    template <typename T>
    void verifyRenderObjectRender(::testing::StrictMock<T>& renderObjectTestable, std::shared_ptr<ShaderMock> shaderMock, bool isDefaultRender, bool checkModelMatrix = false, unsigned int useCallTimes = 1)
    {
        // Create normal shader
        std::shared_ptr<ShaderMock> normalShaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        
        // Create projection and view matrices
        std::array<float, MathUtils::MAT4_SIZE> projectionMatrix{ 1.0f, 1.0f, 1.0f, 1.0f,
                                                                 1.0f, 1.0f, 1.0f, 1.0f,
                                                                 1.0f, 1.0f, 1.0f, 1.0f,
                                                                 1.0f, 1.0f, 1.0f, 1.0f };
        std::array<float, MathUtils::MAT4_SIZE> viewMatrix{ 2.0f, 2.0f, 2.0f, 2.0f,
                                                            2.0f, 2.0f, 2.0f, 2.0f,
                                                            2.0f, 2.0f, 2.0f, 2.0f,
                                                            2.0f, 2.0f, 2.0f, 2.0f };

        // Set expectations on the mock object with parameter verification
        std::shared_ptr<ShaderMock> shaderToUse = (isDefaultRender) ? shaderMock : normalShaderMock;
        EXPECT_CALL(*shaderToUse, use()).Times(useCallTimes);
        EXPECT_CALL(*shaderToUse, setMat4(::testing::StrEq("projection"), FloatArrayEq(projectionMatrix.data()))).Times(1);
        EXPECT_CALL(*shaderToUse, setMat4(::testing::StrEq("view"), FloatArrayEq(viewMatrix.data()))).Times(1);

        // Expect based on whether default or normal render
        if (checkModelMatrix)
            EXPECT_CALL(*shaderToUse, setMat4(::testing::StrEq("model"), FloatArrayEq(ZERO_MATRIX.data()))).Times(1);

        // Call correct render function based on operation to perform
        if (isDefaultRender)
            renderObjectTestable.render(projectionMatrix, viewMatrix); // FUT
        else
            renderObjectTestable.drawNormals(normalShaderMock, projectionMatrix, viewMatrix); // FUT
    }

    template <typename T>
    void verifyInstancedConstructor(::testing::StrictMock<T>& renderObjectTestable)
    {
        // RenderObject inherited properties
        verifyRenderObjectConstructorSetsShader(renderObjectTestable);

        // Verify member properties
        EXPECT_TRUE(renderObjectTestable.getRasterizer() != nullptr);
        EXPECT_TRUE(renderObjectTestable.getVAO() == 0); // RasterizerMock sets to 0
        EXPECT_TRUE(renderObjectTestable.getModelMatrices().size() == 0);
        EXPECT_TRUE(renderObjectTestable.getTextureIDs().size() == 0);
    }

    template <typename T>
    void verifyAddInstancedObject(::testing::StrictMock<T>& renderObjectTestable, InstancedRasterizerMock& instancedRasterizerMock, unsigned int expectedSize)
    {
        float textureID = 1.0f;

        std::vector< std::array<float, MathUtils::MAT4_SIZE>> expectedModelMatrix;
        expectedModelMatrix.resize(expectedSize); std::fill(expectedModelMatrix.begin(), expectedModelMatrix.end(), IDENTITY_MATRIX);
        EXPECT_CALL(instancedRasterizerMock, updateModelMatrices(FloatVectorArrayEq(expectedModelMatrix))).Times(1);
        std::vector<float> expectedTextureLayerIDs;
        expectedTextureLayerIDs.resize(expectedSize); std::fill(expectedTextureLayerIDs.begin(), expectedTextureLayerIDs.end(), 1.0f);
        EXPECT_CALL(instancedRasterizerMock, updateTextureLayerIDs(FloatVectorEq(expectedTextureLayerIDs))).Times(1);

        renderObjectTestable.addInstancedObject(IDENTITY_MATRIX, textureID); // FUT

        EXPECT_TRUE(renderObjectTestable.getModelMatrices().size() == expectedSize);
        EXPECT_EQ(IDENTITY_MATRIX, renderObjectTestable.getModelMatrices().at(expectedSize-1));
        EXPECT_TRUE(renderObjectTestable.getTextureIDs().size() == expectedSize);
        EXPECT_EQ(textureID, renderObjectTestable.getTextureIDs().at(expectedSize-1));
    }

    template <typename T>
    void verifyRemoveInstancedObject(::testing::StrictMock<T>& renderObjectTestable, InstancedRasterizerMock& instancedRasterizerMock, unsigned int startingSize)
    {
        float textureID = 1.0f;
        
        verifyAddInstancedObject(renderObjectTestable, instancedRasterizerMock, startingSize);

        // Confirm removing invalid index doesn't do anything
        renderObjectTestable.removeInstancedObject(100); // FUT

        // Confirm removing valid index removes it from instanced array
        std::vector< std::array<float, MathUtils::MAT4_SIZE>> expectedModelMatrix{ IDENTITY_MATRIX };
        EXPECT_CALL(instancedRasterizerMock, updateModelMatrices(FloatVectorArrayEq(expectedModelMatrix))).Times(1);
        std::vector<float> expectedTextureLayerIDs{ textureID };
        EXPECT_CALL(instancedRasterizerMock, updateTextureLayerIDs(FloatVectorEq(expectedTextureLayerIDs))).Times(1);

        renderObjectTestable.removeInstancedObject(startingSize-1); // FUT

        EXPECT_TRUE(renderObjectTestable.getModelMatrices().size() == startingSize-1);
        EXPECT_TRUE(renderObjectTestable.getTextureIDs().size() == startingSize-1);
    }

    template <typename T>
    void verifyNonInstancedConstructor(::testing::StrictMock<T>& renderObjectTestable, ObjectLocationOrientation& objectLocation)
    {
        // RenderObject inherited properties
        verifyRenderObjectConstructorSetsShader(renderObjectTestable);

        // Verify member properties
        EXPECT_TRUE(renderObjectTestable.getRasterizer() != nullptr);
        EXPECT_TRUE(renderObjectTestable.getVAO() == 0); // RasterizerMock sets to 0
        EXPECT_EQ(renderObjectTestable.getObjectLocationOrientation(), objectLocation);
    }

    template <typename T>
    void verifyNonInstancedRender(::testing::StrictMock<T>& renderObjectTestable, TransformMock& transformMock, ObjectLocationOrientation& objectLocation, NonInstancedRasterizerMock& nonInstancedRasterizerMock, std::shared_ptr<ShaderMock> shaderMock, bool isDefaultRender, unsigned int useCallTimes)
    {
        // Verify variables and method calls
        EXPECT_CALL(transformMock, resetMatrix(::testing::Eq(MatrixTransform::MatrixTypes::MODEL))).Times(1);
        EXPECT_CALL(transformMock, applyTranslation(::testing::Eq(MatrixTransform::MatrixTypes::MODEL), ::testing::Eq(objectLocation.position))).Times(1);
        EXPECT_CALL(transformMock, applyScale(::testing::Eq(MatrixTransform::MatrixTypes::MODEL), ::testing::Eq(objectLocation.scale))).Times(1);
        EXPECT_CALL(transformMock, applyRotation(::testing::Eq(MatrixTransform::MatrixTypes::MODEL), ::testing::Eq(objectLocation.orientation), ::testing::Eq(objectLocation.position))).Times(1);
        EXPECT_CALL(transformMock, getTransformationMatrix(::testing::Eq(MatrixTransform::MatrixTypes::MODEL))).Times(1);
        EXPECT_CALL(nonInstancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

        verifyRenderObjectRender(renderObjectTestable, shaderMock, isDefaultRender, true, useCallTimes);
    }
}

// ===============================================================
// RenderObjectsTestable
// ===============================================================
class RenderObjectsTestable : public RenderObject {
public:
    RenderObjectsTestable(std::shared_ptr<Shader> shaderPtr)
        : RenderObject(shaderPtr) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }

};

// ------------------------------------------------------------------------
class RenderObjectTest : public testing::Test
{
public:
    static std::shared_ptr<ShaderMock> s_shaderMock;
    static ::testing::StrictMock<RenderObjectsTestable>* s_renderObjectTestable;

protected:
    static void SetUpTestCase()
    {
        s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        s_renderObjectTestable = new ::testing::StrictMock<RenderObjectsTestable>(s_shaderMock);
    }

    static void TearDownTestCase()
    {
        s_shaderMock.reset();
        delete s_renderObjectTestable;
    }
};

std::shared_ptr<ShaderMock> RenderObjectTest::s_shaderMock = nullptr;
::testing::StrictMock<RenderObjectsTestable>* RenderObjectTest::s_renderObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(RenderObjectTest, CreateRenderObjectStoresShaderAsMember)
{
    verifyRenderObjectConstructorSetsShader(*s_renderObjectTestable);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    verifyRenderObjectRender(*s_renderObjectTestable, s_shaderMock, true);
}

// ------------------------------------------------------------------------
TEST_F(RenderObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    verifyRenderObjectRender(*s_renderObjectTestable, s_shaderMock, false);
}


// ===============================================================
// InstancedObjectsTestable
// ===============================================================
class InstancedObjectsTestable : public InstancedObject {
public:
    InstancedObjectsTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<InstancedRasterizer> rasterizer)
        : InstancedObject(shaderPtr, rasterizer) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    unsigned int getVAO() { return m_VAO; }
    std::vector<std::array<float, MathUtils::MAT4_SIZE>> getModelMatrices() { return m_modelMatrices; }
    std::vector<float> getTextureIDs() { return m_textureIDs; }
    std::shared_ptr<InstancedRasterizer> getRasterizer() { return m_rasterizer; }

};

// ------------------------------------------------------------------------
class InstancedObjectTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<InstancedRasterizerMock> s_instancedRasterizerMock;
    static ::testing::StrictMock<InstancedObjectsTestable>* s_instancedObjectTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        s_instancedRasterizerMock = std::make_shared<InstancedRasterizerMock>(vertexProperties, std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{});

        EXPECT_CALL(*s_instancedRasterizerMock, createNewVAO()).Times(1);

        s_instancedObjectTestable = new ::testing::StrictMock<InstancedObjectsTestable>(RenderObjectTest::s_shaderMock, s_instancedRasterizerMock);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        s_instancedRasterizerMock.reset();
        delete s_instancedObjectTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<InstancedRasterizerMock> InstancedObjectTest::s_instancedRasterizerMock = nullptr;
::testing::StrictMock<InstancedObjectsTestable>* InstancedObjectTest::s_instancedObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(InstancedObjectTest, CreateInstancedObjectCorrectlySetDefaultProperties)
{
    verifyInstancedConstructor(*s_instancedObjectTestable);
}

// ------------------------------------------------------------------------
TEST_F(InstancedObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    // Verify inherited and member behavior
    EXPECT_CALL(*s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_instancedObjectTestable, RenderObjectTest::s_shaderMock, true);
}

// ------------------------------------------------------------------------
TEST_F(InstancedObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    // Verify inherited and member behavior
    EXPECT_CALL(*s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_instancedObjectTestable, RenderObjectTest::s_shaderMock, false);
}

// ------------------------------------------------------------------------
TEST_F(InstancedObjectTest, AddInstancedObjectCorrectlyAddsModelMatrixAndTextureID)
{
    verifyAddInstancedObject(*s_instancedObjectTestable, *s_instancedRasterizerMock, 1);
}

// ------------------------------------------------------------------------
TEST_F(InstancedObjectTest, RemoveInstancedObjectCorrectlyDeletesModelMatrixAndTextureID)
{
    verifyRemoveInstancedObject(*s_instancedObjectTestable, *s_instancedRasterizerMock, 2);
}

// ------------------------------------------------------------------------
TEST_F(InstancedObjectTest, UpdateInstancedObjectCorrectlyUpdatesModelMatrixAndTextureIdAtIndex)
{
    // TODO: Implement
    int index = 0;
    MathUtils::Vec3 newLocation(1.0f, 2.0f, 3.0f);

    EXPECT_CALL(*s_instancedRasterizerMock, updateModelMatrices(::testing::_)).Times(1);

    // FUT
    s_instancedObjectTestable->updateInstancedObject(index, newLocation);

    std::array<float, MathUtils::MAT4_SIZE> modelMatrix = s_instancedObjectTestable->getModelMatrices().at(0);

    EXPECT_EQ(modelMatrix.at(12), newLocation.x);
    EXPECT_EQ(modelMatrix.at(13), newLocation.y);
    EXPECT_EQ(modelMatrix.at(14), newLocation.z);
}


// ===============================================================
// NonInstancedObjectsTestable
// ===============================================================
class NonInstancedObjectsTestable : public NonInstancedObject {
public:
    NonInstancedObjectsTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<NonInstancedRasterizer> rasterizer, ObjectLocationOrientation objectLocation)
        : NonInstancedObject(shaderPtr, rasterizer, objectLocation) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    unsigned int getVAO() { return m_VAO; }
    std::shared_ptr<NonInstancedRasterizer> getRasterizer() { return m_rasterizer; }
    ObjectLocationOrientation getObjectLocationOrientation() { return m_objectLocation; }
    void setTransformObject(const std::shared_ptr<Transform> transform) { NonInstancedObject::setTransform(transform); }

};

// ------------------------------------------------------------------------
class NonInstancedObjectTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<NonInstancedRasterizerMock> s_nonInstancedRasterizerMock;
    static std::shared_ptr<ObjectLocationOrientation> s_objectLocation;
    static std::shared_ptr<TransformMock> s_transformMock;
    static ::testing::StrictMock<NonInstancedObjectsTestable>* s_nonInstancedObjectTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        s_nonInstancedRasterizerMock = std::make_shared<NonInstancedRasterizerMock>(vertexProperties);
        s_objectLocation = std::make_shared<ObjectLocationOrientation>(MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f);
        s_transformMock = std::make_shared<TransformMock>();

        EXPECT_CALL(*s_nonInstancedRasterizerMock, createNewVAO()).Times(1);

        s_nonInstancedObjectTestable = new ::testing::StrictMock<NonInstancedObjectsTestable>(RenderObjectTest::s_shaderMock, s_nonInstancedRasterizerMock, *s_objectLocation);

        s_nonInstancedObjectTestable->setTransformObject(s_transformMock);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        s_objectLocation.reset();
        s_transformMock.reset();
        s_nonInstancedRasterizerMock.reset();
        delete s_nonInstancedObjectTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<NonInstancedRasterizerMock> NonInstancedObjectTest::s_nonInstancedRasterizerMock = nullptr;
std::shared_ptr<ObjectLocationOrientation> NonInstancedObjectTest::s_objectLocation;
std::shared_ptr<TransformMock> NonInstancedObjectTest::s_transformMock;
::testing::StrictMock<NonInstancedObjectsTestable>* NonInstancedObjectTest::s_nonInstancedObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(NonInstancedObjectTest, CreateNonInstancedObjectCorrectlySetDefaultProperties)
{
    verifyNonInstancedConstructor(*s_nonInstancedObjectTestable, *s_objectLocation);
}

// ------------------------------------------------------------------------
TEST_F(NonInstancedObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    verifyNonInstancedRender(*s_nonInstancedObjectTestable, *s_transformMock, *s_objectLocation, *s_nonInstancedRasterizerMock, RenderObjectTest::s_shaderMock, true, 2);
}

// ------------------------------------------------------------------------
TEST_F(NonInstancedObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    verifyNonInstancedRender(*s_nonInstancedObjectTestable, *s_transformMock, *s_objectLocation, *s_nonInstancedRasterizerMock, RenderObjectTest::s_shaderMock, false, 2);
}


// ===============================================================
// GeometricInstancedObjectsTestable
// ===============================================================
class GeometricInstancedObjectsTestable : public GeometricInstancedObject {
public:
    GeometricInstancedObjectsTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<InstancedRasterizer> rasterizer, ObjectMaterialProperties objectMaterialProperties)
        : GeometricInstancedObject(shaderPtr, rasterizer, objectMaterialProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    unsigned int getVAO() { return m_VAO; }
    std::vector<std::array<float, MathUtils::MAT4_SIZE>> getModelMatrices() { return m_modelMatrices; }
    std::vector<float> getTextureIDs() { return m_textureIDs; }
    std::shared_ptr<InstancedRasterizer> getRasterizer() { return m_rasterizer; }
    ObjectMaterialProperties getObjectMaterialProperties() { return m_objectMaterialProperties; }

};

// ------------------------------------------------------------------------
class GeometricInstancedObjectTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<ObjectMaterialProperties> s_objectMaterialProperties;
    static ::testing::StrictMock<GeometricInstancedObjectsTestable>* s_geometricInstancedObjectTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        InstancedObjectTest::s_instancedRasterizerMock = std::make_shared<InstancedRasterizerMock>(vertexProperties, std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{});
        s_objectMaterialProperties = std::make_shared<ObjectMaterialProperties>(32.0f, 0, 1);

        EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, createNewVAO()).Times(1);

        s_geometricInstancedObjectTestable = new ::testing::StrictMock<GeometricInstancedObjectsTestable>(RenderObjectTest::s_shaderMock, InstancedObjectTest::s_instancedRasterizerMock, *s_objectMaterialProperties);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        s_objectMaterialProperties.reset();
        InstancedObjectTest::s_instancedRasterizerMock.reset();
        delete s_geometricInstancedObjectTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<ObjectMaterialProperties> GeometricInstancedObjectTest::s_objectMaterialProperties = nullptr;
::testing::StrictMock<GeometricInstancedObjectsTestable>* GeometricInstancedObjectTest::s_geometricInstancedObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(GeometricInstancedObjectTest, CreateGeometricInstancedObjectCorrectlySetDefaultProperties)
{
    verifyInstancedConstructor(*s_geometricInstancedObjectTestable);

    EXPECT_EQ(*s_objectMaterialProperties, s_geometricInstancedObjectTestable->getObjectMaterialProperties());
}

// ------------------------------------------------------------------------
TEST_F(GeometricInstancedObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    // Verify inherited behavior
    TextureMaps textureMaps = { s_objectMaterialProperties->diffuseMap, s_objectMaterialProperties->specularMap };
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, activateTextures(::testing::Eq(textureMaps))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("material.diffuseMapArray"), ::testing::Eq(s_objectMaterialProperties->diffuseMap))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("material.specularMap"), ::testing::Eq(s_objectMaterialProperties->specularMap))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("material.shininess"), ::testing::FloatEq(s_objectMaterialProperties->shininess))).Times(1);
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_geometricInstancedObjectTestable, RenderObjectTest::s_shaderMock, true, false, 2);
}

// ------------------------------------------------------------------------
TEST_F(GeometricInstancedObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    // Verify inherited behavior
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_geometricInstancedObjectTestable, RenderObjectTest::s_shaderMock, false, false, 1);
}

// ------------------------------------------------------------------------
TEST_F(GeometricInstancedObjectTest, AddGeometricInstancedObjectCorrectlyAddsModelMatrixAndTextureID)
{
    verifyAddInstancedObject(*s_geometricInstancedObjectTestable, *InstancedObjectTest::s_instancedRasterizerMock, 1);
}

// ------------------------------------------------------------------------
TEST_F(GeometricInstancedObjectTest, RemoveGeometricInstancedObjectCorrectlyDeletesModelMatrixAndTextureID)
{
    verifyRemoveInstancedObject(*s_geometricInstancedObjectTestable, *InstancedObjectTest::s_instancedRasterizerMock, 2);
}


// ===============================================================
// GeometricNonInstancedObjectsTestable
// ===============================================================
class GeometricNonInstancedObjectsTestable : public GeometricNonInstancedObject {
public:
    GeometricNonInstancedObjectsTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<NonInstancedRasterizer> rasterizer, ObjectLocationOrientation objectLocation, ObjectMaterialProperties objectMaterialProperties)
        : GeometricNonInstancedObject(shaderPtr, rasterizer, objectLocation, objectMaterialProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    unsigned int getVAO() { return m_VAO; }
    std::shared_ptr<NonInstancedRasterizer> getRasterizer() { return m_rasterizer; }
    ObjectLocationOrientation getObjectLocationOrientation() { return m_objectLocation; }
    void setTransformObject(const std::shared_ptr<Transform> transform) { NonInstancedObject::setTransform(transform); }
    ObjectMaterialProperties getObjectMaterialProperties() { return m_objectMaterialProperties; }

};

// ------------------------------------------------------------------------
class GeometricNonInstancedObjectTest : public OpenGLTestFixture
{
public:
    static ::testing::StrictMock<GeometricNonInstancedObjectsTestable>* s_geometricNonInstancedObjectTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        NonInstancedObjectTest::s_nonInstancedRasterizerMock = std::make_shared<NonInstancedRasterizerMock>(vertexProperties);
        NonInstancedObjectTest::s_objectLocation = std::make_shared<ObjectLocationOrientation>(MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f);
        NonInstancedObjectTest::s_transformMock = std::make_shared<TransformMock>();
        GeometricInstancedObjectTest::s_objectMaterialProperties = std::make_shared<ObjectMaterialProperties>(32.0f, 0, 1);

        EXPECT_CALL(*NonInstancedObjectTest::s_nonInstancedRasterizerMock, createNewVAO()).Times(1);
                                               
        s_geometricNonInstancedObjectTestable = new ::testing::StrictMock<GeometricNonInstancedObjectsTestable>(RenderObjectTest::s_shaderMock, NonInstancedObjectTest::s_nonInstancedRasterizerMock, *NonInstancedObjectTest::s_objectLocation, *GeometricInstancedObjectTest::s_objectMaterialProperties);

        s_geometricNonInstancedObjectTestable->setTransformObject(NonInstancedObjectTest::s_transformMock);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        NonInstancedObjectTest::s_objectLocation.reset();
        NonInstancedObjectTest::s_transformMock.reset();
        NonInstancedObjectTest::s_nonInstancedRasterizerMock.reset();
        GeometricInstancedObjectTest::s_objectMaterialProperties.reset();
        delete s_geometricNonInstancedObjectTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

::testing::StrictMock<GeometricNonInstancedObjectsTestable>* GeometricNonInstancedObjectTest::s_geometricNonInstancedObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(GeometricNonInstancedObjectTest, CreateNonInstancedObjectCorrectlySetDefaultProperties)
{
    verifyNonInstancedConstructor(*s_geometricNonInstancedObjectTestable, *NonInstancedObjectTest::s_objectLocation);
}

// ------------------------------------------------------------------------
TEST_F(GeometricNonInstancedObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    TextureMaps textureMaps = { GeometricInstancedObjectTest::s_objectMaterialProperties->diffuseMap, GeometricInstancedObjectTest::s_objectMaterialProperties->specularMap };
    EXPECT_CALL(*NonInstancedObjectTest::s_nonInstancedRasterizerMock, activateTextures(::testing::Eq(textureMaps))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("material.diffuseMap"), ::testing::Eq(GeometricInstancedObjectTest::s_objectMaterialProperties->diffuseMap))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("material.specularMap"), ::testing::Eq(GeometricInstancedObjectTest::s_objectMaterialProperties->specularMap))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("material.shininess"), ::testing::FloatEq(GeometricInstancedObjectTest::s_objectMaterialProperties->shininess))).Times(1);

    verifyNonInstancedRender(*s_geometricNonInstancedObjectTestable, *NonInstancedObjectTest::s_transformMock, *NonInstancedObjectTest::s_objectLocation, *NonInstancedObjectTest::s_nonInstancedRasterizerMock, RenderObjectTest::s_shaderMock, true, 3);
}

// ------------------------------------------------------------------------
TEST_F(GeometricNonInstancedObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    verifyNonInstancedRender(*s_geometricNonInstancedObjectTestable, *NonInstancedObjectTest::s_transformMock, *NonInstancedObjectTest::s_objectLocation, *NonInstancedObjectTest::s_nonInstancedRasterizerMock, RenderObjectTest::s_shaderMock, false, 2);
}


// ===============================================================
// LightObjectTestable
// ===============================================================
class LightObjectTestable : public LightObject {
public:
    LightObjectTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<InstancedRasterizer> rasterizer, PhongLightingParams phongLightProperties)
        : LightObject(shaderPtr, rasterizer, phongLightProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    unsigned int getVAO() { return m_VAO; }
    std::vector<std::array<float, MathUtils::MAT4_SIZE>> getModelMatrices() { return m_modelMatrices; }
    std::vector<float> getTextureIDs() { return m_textureIDs; }
    std::shared_ptr<InstancedRasterizer> getRasterizer() { return m_rasterizer; }
    PhongLightingParams getPhongLightParams() { return m_phongLightParams; }

};

// ------------------------------------------------------------------------
class LightObjectTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<PhongLightingParams> s_phongLightingParams;
    static ::testing::StrictMock<LightObjectTestable>* s_lightObjectTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        InstancedObjectTest::s_instancedRasterizerMock = std::make_shared<InstancedRasterizerMock>(vertexProperties, std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{});
        s_phongLightingParams = std::make_shared<PhongLightingParams>(MathUtils::Vec3(1.0f, 1.0f, 1.0f), MathUtils::Vec3(2.0f, 2.0f, 2.0f), MathUtils::Vec3(3.0f, 3.0f, 3.0f));

        EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, createNewVAO()).Times(1);

        s_lightObjectTestable = new ::testing::StrictMock<LightObjectTestable>(RenderObjectTest::s_shaderMock, InstancedObjectTest::s_instancedRasterizerMock, *s_phongLightingParams);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        InstancedObjectTest::s_instancedRasterizerMock.reset();
        delete s_lightObjectTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<PhongLightingParams> LightObjectTest::s_phongLightingParams = nullptr;
::testing::StrictMock<LightObjectTestable>* LightObjectTest::s_lightObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(LightObjectTest, CreateInstancedObjectCorrectlySetDefaultProperties)
{
    // Test inherited members
    verifyInstancedConstructor(*s_lightObjectTestable);

    // Test class-specific members
    EXPECT_EQ(s_lightObjectTestable->getPhongLightParams(), *s_phongLightingParams);
}

// ------------------------------------------------------------------------
TEST_F(LightObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    // Verify inherited and member behavior
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_lightObjectTestable, RenderObjectTest::s_shaderMock, true);
}

// ------------------------------------------------------------------------
TEST_F(LightObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    // Verify inherited and member behavior
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_lightObjectTestable, RenderObjectTest::s_shaderMock, false);
}

// ------------------------------------------------------------------------
TEST_F(LightObjectTest, AddInstancedObjectCorrectlyAddsModelMatrixAndTextureID)
{
    verifyAddInstancedObject(*s_lightObjectTestable, *InstancedObjectTest::s_instancedRasterizerMock, 1);
}

// ------------------------------------------------------------------------
TEST_F(LightObjectTest, RemoveInstancedObjectCorrectlyDeletesModelMatrixAndTextureID)
{
    verifyRemoveInstancedObject(*s_lightObjectTestable, *InstancedObjectTest::s_instancedRasterizerMock, 2);
}



// ===============================================================
// PointObjectTestable
// ===============================================================
class PointObjectTestable : public PointLight {
public:
    PointObjectTestable(std::vector<std::shared_ptr<Shader>> shaderPtrs, std::shared_ptr<InstancedRasterizer> rasterizer, ObjectMaterialProperties objectMaterialProperties, PhongLightingParams phongLightProperties, AttenuationParams attenuationParams)
        : PointLight(shaderPtrs, rasterizer, objectMaterialProperties, phongLightProperties, attenuationParams) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    unsigned int getVAO() { return m_VAO; }
    std::vector<std::array<float, MathUtils::MAT4_SIZE>> getModelMatrices() { return m_modelMatrices; }
    std::vector<float> getTextureIDs() { return m_textureIDs; }
    std::shared_ptr<InstancedRasterizer> getRasterizer() { return m_rasterizer; }
    PhongLightingParams getPhongLightParams() { return m_phongLightParams; }
    ObjectMaterialProperties getObjectMaterialProperties() { return m_objectMaterialProperties; }
    AttenuationParams getAttenuationParams() { return m_attenuationParams; }
    std::vector<std::shared_ptr<Shader>> getShaders() { return m_shaders; }

};

// ------------------------------------------------------------------------
class PointLightTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<AttenuationParams> s_attenuationParams;
    static ::testing::StrictMock<PointObjectTestable>* s_pointObjectTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        InstancedObjectTest::s_instancedRasterizerMock = std::make_shared<InstancedRasterizerMock>(vertexProperties, std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{});
        LightObjectTest::s_phongLightingParams = std::make_shared<PhongLightingParams>(MathUtils::Vec3(1.0f, 1.0f, 1.0f), MathUtils::Vec3(2.0f, 2.0f, 2.0f), MathUtils::Vec3(3.0f, 3.0f, 3.0f));
        GeometricInstancedObjectTest::s_objectMaterialProperties = std::make_shared<ObjectMaterialProperties>(32.0f, 0, 1);
        s_attenuationParams = std::make_shared<AttenuationParams>(1.0f, 0.09f, 0.032f);

        EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, createNewVAO()).Times(1);

        s_pointObjectTestable = new ::testing::StrictMock<PointObjectTestable>(std::vector<std::shared_ptr<Shader>>{RenderObjectTest::s_shaderMock, RenderObjectTest::s_shaderMock}, InstancedObjectTest::s_instancedRasterizerMock, *GeometricInstancedObjectTest::s_objectMaterialProperties, *LightObjectTest::s_phongLightingParams, *s_attenuationParams);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        InstancedObjectTest::s_instancedRasterizerMock.reset();
        delete s_pointObjectTestable;

        OpenGLTestFixture::TearDownTestCase();
    }
};

std::shared_ptr<AttenuationParams> PointLightTest::s_attenuationParams = nullptr;
::testing::StrictMock<PointObjectTestable>* PointLightTest::s_pointObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(PointLightTest, CreateInstancedObjectCorrectlySetDefaultProperties)
{
    // Test inherited members
    verifyInstancedConstructor(*s_pointObjectTestable);

    // Test class-specific members
    EXPECT_EQ(s_pointObjectTestable->getPhongLightParams(), *LightObjectTest::s_phongLightingParams);
}

// ------------------------------------------------------------------------
TEST_F(PointLightTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    // Verify inherited and member behavior
    TextureMaps textureMaps = { GeometricInstancedObjectTest::s_objectMaterialProperties->diffuseMap, GeometricInstancedObjectTest::s_objectMaterialProperties->specularMap };
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, activateTextures(::testing::Eq(textureMaps))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("material.diffuseMapArray"), ::testing::Eq(GeometricInstancedObjectTest::s_objectMaterialProperties->diffuseMap))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("material.specularMap"), ::testing::Eq(GeometricInstancedObjectTest::s_objectMaterialProperties->specularMap))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("material.shininess"), ::testing::FloatEq(GeometricInstancedObjectTest::s_objectMaterialProperties->shininess))).Times(1);
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_pointObjectTestable, RenderObjectTest::s_shaderMock, true, false, 2);
}

// ------------------------------------------------------------------------
TEST_F(PointLightTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    // Verify inherited and member behavior
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    verifyRenderObjectRender(*s_pointObjectTestable, RenderObjectTest::s_shaderMock, false);
}

// ------------------------------------------------------------------------
TEST_F(PointLightTest, AddInstancedObjectCorrectlyAddsModelMatrixAndTextureID)
{
    verifyAddInstancedObject(*s_pointObjectTestable, *InstancedObjectTest::s_instancedRasterizerMock, 1);
}

// ------------------------------------------------------------------------
TEST_F(PointLightTest, RemoveInstancedObjectCorrectlyDeletesModelMatrixAndTextureID)
{
    verifyRemoveInstancedObject(*s_pointObjectTestable, *InstancedObjectTest::s_instancedRasterizerMock, 2);
}

// ------------------------------------------------------------------------
TEST_F(PointLightTest, UpdatePointLightProperties)
{
    // Confirm shader-specific values
    int numShaderCalls = static_cast<int>(s_pointObjectTestable->getShaders().size());
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(numShaderCalls);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("numPointLights"), ::testing::Eq(s_pointObjectTestable->getTextureIDs().size()))).Times(numShaderCalls);
    TextureMaps textureMaps = { GeometricInstancedObjectTest::s_objectMaterialProperties->diffuseMap, GeometricInstancedObjectTest::s_objectMaterialProperties->specularMap };
    EXPECT_CALL(*InstancedObjectTest::s_instancedRasterizerMock, activateTextures(::testing::Eq(textureMaps))).Times(numShaderCalls);
    
    // Confirm texture-specific values
    int numTextureIDUpdates = static_cast<int>(s_pointObjectTestable->getShaders().size() * s_pointObjectTestable->getTextureIDs().size());
    MathUtils::Vec3 expectedPosition{ s_pointObjectTestable->getModelMatrices().at(0)[12], s_pointObjectTestable->getModelMatrices().at(0)[13], s_pointObjectTestable->getModelMatrices().at(0)[14]}; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("pointLights[0].position"), FloatVec3Eq(&expectedPosition.x))).Times(numTextureIDUpdates);
    MathUtils::Vec3 expectedAmbient = s_pointObjectTestable->getPhongLightParams().ambient; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("pointLights[0].ambient"), FloatVec3Eq(&expectedAmbient.x))).Times(numTextureIDUpdates);
    MathUtils::Vec3 expectedDiffuse = s_pointObjectTestable->getPhongLightParams().diffuse; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("pointLights[0].diffuse"), FloatVec3Eq(&expectedDiffuse.x))).Times(numTextureIDUpdates);
    MathUtils::Vec3 expectedSpecular = s_pointObjectTestable->getPhongLightParams().specular; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("pointLights[0].specular"), FloatVec3Eq(&expectedSpecular.x))).Times(numTextureIDUpdates);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("pointLights[0].constant"), ::testing::FloatEq(s_pointObjectTestable->getAttenuationParams().constant))).Times(numTextureIDUpdates);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("pointLights[0].linear"), ::testing::FloatEq(s_pointObjectTestable->getAttenuationParams().linear))).Times(numTextureIDUpdates);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("pointLights[0].quadratic"), ::testing::FloatEq(s_pointObjectTestable->getAttenuationParams().quadratic))).Times(numTextureIDUpdates);
    
    s_pointObjectTestable->updateLightProperties(); // FUT
}


// ===============================================================
// DirectionalLightTestable
// ===============================================================
class DirectionalLightTestable : public DirectionalLight {
public:
    DirectionalLightTestable(std::vector<std::shared_ptr<Shader>> shaderPtrs, LightProperties lightProperties, PhongLightingParams phongLightingParams)
        : DirectionalLight(shaderPtrs, lightProperties, phongLightingParams) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    std::vector<std::shared_ptr<Shader>> getShaders() { return m_shaders; }

};

// ------------------------------------------------------------------------
class DirectionalLightTest : public testing::Test
{
public:
    static std::shared_ptr<LightProperties> s_lightProperties;
    static ::testing::StrictMock<DirectionalLightTestable>* s_directionalLightTestable;

protected:
    static void SetUpTestCase()
    {
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        LightObjectTest::s_phongLightingParams = std::make_shared<PhongLightingParams>(MathUtils::Vec3(1.0f, 1.0f, 1.0f), MathUtils::Vec3(2.0f, 2.0f, 2.0f), MathUtils::Vec3(3.0f, 3.0f, 3.0f));
        s_lightProperties = std::make_shared<LightProperties>(MathUtils::Vec3(0.0f, -1.0f, 0.0f), MathUtils::Vec3(0.0f, -1.0f, 0.0f));

        // setDirectionLightParams called in constructor, expect calls
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(2);
        MathUtils::Vec3 expectedDirection = s_lightProperties->direction; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("directionalLight.direction"), FloatVec3Eq(&expectedDirection.x))).Times(2);
        MathUtils::Vec3 expectedAmbient = LightObjectTest::s_phongLightingParams->ambient; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("directionalLight.ambient"), FloatVec3Eq(&expectedAmbient.x))).Times(2);
        MathUtils::Vec3 expectedDiffuse = LightObjectTest::s_phongLightingParams->diffuse; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("directionalLight.diffuse"), FloatVec3Eq(&expectedDiffuse.x))).Times(2);
        MathUtils::Vec3 expectedSpecular = LightObjectTest::s_phongLightingParams->specular; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("directionalLight.specular"), FloatVec3Eq(&expectedSpecular.x))).Times(2);

        s_directionalLightTestable = new ::testing::StrictMock<DirectionalLightTestable>(std::vector<std::shared_ptr<Shader>>{RenderObjectTest::s_shaderMock, RenderObjectTest::s_shaderMock}, *s_lightProperties, *LightObjectTest::s_phongLightingParams);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        LightObjectTest::s_phongLightingParams.reset();
        s_lightProperties.reset();
        delete s_directionalLightTestable;
    }
};

std::shared_ptr<LightProperties> DirectionalLightTest::s_lightProperties = nullptr;
::testing::StrictMock<DirectionalLightTestable>* DirectionalLightTest::s_directionalLightTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(DirectionalLightTest, CreateRenderObjectStoresShaderAsMember)
{
    verifyRenderObjectConstructorSetsShader(*s_directionalLightTestable);
}

// ------------------------------------------------------------------------
TEST_F(DirectionalLightTest, RenderDoesNotSetAnything)
{
    s_directionalLightTestable->render(IDENTITY_MATRIX, IDENTITY_MATRIX); // FUT
}

// ------------------------------------------------------------------------
TEST_F(DirectionalLightTest, DrawNormalDoesNotSetAnything)
{
    verifyRenderObjectRender(*s_directionalLightTestable, RenderObjectTest::s_shaderMock, false);
}


// ===============================================================
// SpotLightTestable
// ===============================================================
class SpotLightTestable : public SpotLight {
public:
    SpotLightTestable(std::vector<std::shared_ptr<Shader>> shaderPtrs, LightProperties lightProperties, PhongLightingParams phongLightingParams, AttenuationParams attenuationParams, SpotlightCutoffParams spotlightCutoffParams)
        : SpotLight(shaderPtrs, lightProperties, phongLightingParams, attenuationParams, spotlightCutoffParams) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    std::vector<std::shared_ptr<Shader>> getShaders() { return m_shaders; }

};

// ------------------------------------------------------------------------
class SpotLightTest : public testing::Test
{
public:
    static std::shared_ptr<AttenuationParams> s_attenuationParams;
    static std::shared_ptr<SpotlightCutoffParams> s_spotlightCutoffParams;
    static ::testing::StrictMock<SpotLightTestable>* s_spotLightTestable;

protected:
    static void SetUpTestCase()
    {
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");
        LightObjectTest::s_phongLightingParams = std::make_shared<PhongLightingParams>(MathUtils::Vec3(1.0f, 1.0f, 1.0f), MathUtils::Vec3(2.0f, 2.0f, 2.0f), MathUtils::Vec3(3.0f, 3.0f, 3.0f));
        DirectionalLightTest::s_lightProperties = std::make_shared<LightProperties>(MathUtils::Vec3(0.0f, -1.0f, 0.0f), MathUtils::Vec3(0.0f, -1.0f, 0.0f));
        s_attenuationParams = std::make_shared<AttenuationParams>(1.0f, 0.09f, 0.032f);
        s_spotlightCutoffParams = std::make_shared<SpotlightCutoffParams>(12.5f, 15.0f);

        // setDirectionLightParams called in constructor, expect calls
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(2);
        MathUtils::Vec3 expectedPosition = DirectionalLightTest::s_lightProperties->position; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.position"), FloatVec3Eq(&expectedPosition.x))).Times(2);
        MathUtils::Vec3 expectedDirection = DirectionalLightTest::s_lightProperties->direction; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.direction"), FloatVec3Eq(&expectedDirection.x))).Times(2);
        MathUtils::Vec3 expectedAmbient = LightObjectTest::s_phongLightingParams->ambient; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.ambient"), FloatVec3Eq(&expectedAmbient.x))).Times(2);
        MathUtils::Vec3 expectedDiffuse = LightObjectTest::s_phongLightingParams->diffuse; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.diffuse"), FloatVec3Eq(&expectedDiffuse.x))).Times(2);
        MathUtils::Vec3 expectedSpecular = LightObjectTest::s_phongLightingParams->specular; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.specular"), FloatVec3Eq(&expectedSpecular.x))).Times(2);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("spotLight.constant"), ::testing::FloatEq(s_attenuationParams->constant))).Times(2);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("spotLight.linear"), ::testing::FloatEq(s_attenuationParams->linear))).Times(2);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("spotLight.quadratic"), ::testing::FloatEq(s_attenuationParams->quadratic))).Times(2);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("spotLight.innerCutOff"), ::testing::FloatEq(glm::cos(glm::radians(s_spotlightCutoffParams->innerCutoff))))).Times(2);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, setFloat(::testing::StrEq("spotLight.outerCutOff"), ::testing::FloatEq(glm::cos(glm::radians(s_spotlightCutoffParams->outerCutoff))))).Times(2);

        s_spotLightTestable = new ::testing::StrictMock<SpotLightTestable>(std::vector<std::shared_ptr<Shader>>{RenderObjectTest::s_shaderMock, RenderObjectTest::s_shaderMock}, *DirectionalLightTest::s_lightProperties, * LightObjectTest::s_phongLightingParams, *s_attenuationParams, *s_spotlightCutoffParams);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        LightObjectTest::s_phongLightingParams.reset();
        DirectionalLightTest::s_lightProperties.reset();
        s_attenuationParams.reset();
        s_spotlightCutoffParams.reset();
        delete s_spotLightTestable;
    }
};

std::shared_ptr<AttenuationParams> SpotLightTest::s_attenuationParams = nullptr;
std::shared_ptr<SpotlightCutoffParams> SpotLightTest::s_spotlightCutoffParams = nullptr;
::testing::StrictMock<SpotLightTestable>* SpotLightTest::s_spotLightTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(SpotLightTest, CreateRenderObjectStoresShaderAsMember)
{
    verifyRenderObjectConstructorSetsShader(*s_spotLightTestable);
}

// ------------------------------------------------------------------------
TEST_F(SpotLightTest, RenderDoesNotSetAnything)
{
    s_spotLightTestable->render(IDENTITY_MATRIX, IDENTITY_MATRIX); // FUT
}

// ------------------------------------------------------------------------
TEST_F(SpotLightTest, DrawNormalDoesNotSetAnything)
{
    verifyRenderObjectRender(*s_spotLightTestable, RenderObjectTest::s_shaderMock, false);
}

// ------------------------------------------------------------------------
TEST_F(SpotLightTest, VerifyUpdatePositionCorrectlySetsShaderInfo)
{
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(2);
    MathUtils::Vec3 expectedPosition = DirectionalLightTest::s_lightProperties->position; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.position"), FloatVec3Eq(&expectedPosition.x))).Times(2);
    MathUtils::Vec3 expectedDirection = DirectionalLightTest::s_lightProperties->direction; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("spotLight.direction"), FloatVec3Eq(&expectedDirection.x))).Times(2);

    s_spotLightTestable->updatePosition(*DirectionalLightTest::s_lightProperties); // FUT
}


// ===============================================================
// SkyboxObjectTestable
// ===============================================================
class SkyboxObjectTestable : public SkyboxObject {
public:
    SkyboxObjectTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<SkyboxRasterizer> skyboxRasterizerPtr, unsigned int textureID)
        : SkyboxObject(shaderPtr, skyboxRasterizerPtr, textureID) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    std::shared_ptr<SkyboxRasterizer> getRasterizer() { return m_rasterizer; }
    unsigned int getTextureID() { return m_textureID; }
    unsigned int getVAO() { return m_VAO; }

};

// ------------------------------------------------------------------------
class SkyboxObjectTest : public testing::Test
{
public:
    static unsigned int s_textureID;
    static std::shared_ptr<SkyboxRasterizerMock> s_skyboxRasterizerMock;
    static ::testing::StrictMock<SkyboxObjectTestable>* s_skyboxObjectTestable;

protected:
    static void SetUpTestCase()
    {
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        s_skyboxRasterizerMock = std::make_shared<SkyboxRasterizerMock>(vertexProperties);
        s_textureID = 1;

        // Expect ctor functions
        EXPECT_CALL(*s_skyboxRasterizerMock, createNewVAO()).Times(1);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(1);
        EXPECT_CALL(*RenderObjectTest::s_shaderMock, setInt(::testing::StrEq("skybox"), ::testing::Eq(0))).Times(1);

        s_skyboxObjectTestable = new ::testing::StrictMock<SkyboxObjectTestable>(RenderObjectTest::s_shaderMock, s_skyboxRasterizerMock, s_textureID);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        s_skyboxRasterizerMock.reset();
        delete s_skyboxObjectTestable;
    }
};

unsigned int SkyboxObjectTest::s_textureID;
std::shared_ptr<SkyboxRasterizerMock> SkyboxObjectTest::s_skyboxRasterizerMock = nullptr;
::testing::StrictMock<SkyboxObjectTestable>* SkyboxObjectTest::s_skyboxObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(SkyboxObjectTest, CreateRenderObjectStoresShaderAsMember)
{
    verifyRenderObjectConstructorSetsShader(*s_skyboxObjectTestable);
}

// ------------------------------------------------------------------------
TEST_F(SkyboxObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    // Expect render calls
    TextureMaps textureMaps = { s_textureID, 0 };
    EXPECT_CALL(*s_skyboxRasterizerMock, activateTextures(::testing::Eq(textureMaps))).Times(1);
    EXPECT_CALL(*s_skyboxRasterizerMock, drawArrays(::testing::_, ::testing::_)).Times(1);

    std::array<float, MathUtils::MAT4_SIZE> projectionMatrix{ 1.0f, 1.0f, 1.0f, 1.0f,
                                                                 1.0f, 1.0f, 1.0f, 1.0f,
                                                                 1.0f, 1.0f, 1.0f, 1.0f,
                                                                 1.0f, 1.0f, 1.0f, 1.0f };
    std::array<float, MathUtils::MAT4_SIZE> viewMatrix{ 2.0f, 2.0f, 2.0f, 0.0f,
                                                        2.0f, 2.0f, 2.0f, 0.0f,
                                                        2.0f, 2.0f, 2.0f, 0.0f,
                                                        0.0f, 0.0f, 0.0f, 0.0f };

    EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setMat4(::testing::StrEq("projection"), FloatArrayEq(projectionMatrix.data()))).Times(1);
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, setMat4(::testing::StrEq("view"), FloatArrayEq(viewMatrix.data()))).Times(1);

    s_skyboxObjectTestable->render(projectionMatrix, viewMatrix); // FUT
}

// ------------------------------------------------------------------------
TEST_F(SkyboxObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    verifyRenderObjectRender(*s_skyboxObjectTestable, RenderObjectTest::s_shaderMock, false);
}


// ===============================================================
// TextObjectTestable
// ===============================================================
class TextObjectTestable : public TextObject {
public:
    TextObjectTestable(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<TextRasterizer> textRasterizerPtr, TextProperties textProperties)
        : TextObject(shaderPtr, textRasterizerPtr, textProperties) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Shader> getShader() { return m_shader; }
    std::shared_ptr<TextRasterizer> getRasterizer() { return m_rasterizer; }
    TextProperties getTextProperties() { return m_textProperties; }

};

// ------------------------------------------------------------------------
class TextObjectTest : public testing::Test
{
public:
    static std::shared_ptr<TextRasterizerMock> s_textRasterizerMock;
    static std::shared_ptr<TextProperties> s_textProperties;
    static ::testing::StrictMock<TextObjectTestable>* s_textObjectTestable;

protected:
    static void SetUpTestCase()
    {
        RenderObjectTest::s_shaderMock = std::make_shared<ShaderMock>("ztest_valid_shader.vert", "ztest_valid_shader.frag", "ztest_valid_shader.geom");

        VertexData vertexProperties{ { 1.0f, 1.0f, 1.0f, 2.0f, 2.0f, 2.0f }, 3 };
        s_textRasterizerMock = std::make_shared<TextRasterizerMock>(vertexProperties);
        s_textProperties = std::make_shared<TextProperties>("Test Text", MathUtils::Vec2(100.0f, 100.0f), 2.0f, MathUtils::Vec3(3.0f, 3.0f, 3.0f));

        // Expect ctor functions
        EXPECT_CALL(*s_textRasterizerMock, createNewVAO(::testing::_)).Times(1);

        s_textObjectTestable = new ::testing::StrictMock<TextObjectTestable>(RenderObjectTest::s_shaderMock, s_textRasterizerMock, *s_textProperties);
    }

    static void TearDownTestCase()
    {
        RenderObjectTest::s_shaderMock.reset();
        s_textRasterizerMock.reset();
        s_textProperties.reset();
        delete s_textObjectTestable;
    }
};

std::shared_ptr<TextRasterizerMock> TextObjectTest::s_textRasterizerMock = nullptr;
std::shared_ptr<TextProperties> TextObjectTest::s_textProperties = nullptr;
::testing::StrictMock<TextObjectTestable>* TextObjectTest::s_textObjectTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(TextObjectTest, CreateRenderObjectStoresShaderAsMember)
{
    // Inherited members
    verifyRenderObjectConstructorSetsShader(*s_textObjectTestable);

    // Verify TextObject members
    EXPECT_EQ(s_textProperties->text, s_textObjectTestable->getTextProperties().text);
    EXPECT_EQ(s_textProperties->position, s_textObjectTestable->getTextProperties().position);
    EXPECT_EQ(s_textProperties->scale, s_textObjectTestable->getTextProperties().scale);
    EXPECT_EQ(s_textProperties->color, s_textObjectTestable->getTextProperties().color);
}

// ------------------------------------------------------------------------
TEST_F(TextObjectTest, RenderSetsProjectionAndViewMatrixForMemberShader)
{
    // Expect render calls
    EXPECT_CALL(*RenderObjectTest::s_shaderMock, use()).Times(1);
    MathUtils::Vec3 expectedColor = s_textProperties->color; EXPECT_CALL(*RenderObjectTest::s_shaderMock, setVec3(::testing::StrEq("textColor"), FloatVec3Eq(&expectedColor.x))).Times(1);
    EXPECT_CALL(*s_textRasterizerMock, drawArrays(::testing::_)).Times(1);

    s_textObjectTestable->render(IDENTITY_MATRIX, IDENTITY_MATRIX);
}

// ------------------------------------------------------------------------
TEST_F(TextObjectTest, DrawNormalSetsProjectionAndViewMatrixForNormalShader)
{
    verifyRenderObjectRender(*s_textObjectTestable, RenderObjectTest::s_shaderMock, false);
}

// ------------------------------------------------------------------------
TEST_F(TextObjectTest, SetTextPropertiesCorrectlyUpdatesTextProperties)
{
    TextProperties textProperties{ "Sample Text", {50.0f, 50.0f}, 2.0f, {1.0f, 1.0f, 1.0f} };
    s_textObjectTestable->updateTextProperties(textProperties); // FUT

    // Verify text properties updated
    EXPECT_EQ(textProperties.text, s_textObjectTestable->getTextProperties().text);
    EXPECT_EQ(textProperties.position, s_textObjectTestable->getTextProperties().position);
    EXPECT_EQ(textProperties.scale, s_textObjectTestable->getTextProperties().scale);
    EXPECT_EQ(textProperties.color, s_textObjectTestable->getTextProperties().color);
}
