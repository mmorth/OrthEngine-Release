#ifndef RENDEROBJECTMOCK_HPP
#define RENDEROBJECTMOCK_HPP

#include <gmock/gmock.h>
#include "RenderObjects/RenderObject.hpp"

class RenderObjectMock : public RenderObject {
public:
    RenderObjectMock(std::shared_ptr<Shader> shaderPtr) : RenderObject(shaderPtr) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, drawNormals, (const std::shared_ptr<Shader> shaderPtr, (std::array<float, MathUtils::MAT4_SIZE> projectionMatrix), (std::array<float, MathUtils::MAT4_SIZE> viewMatrix)), (override));
};

class NonInstancedObjectMock : public NonInstancedObject 
{
public:
    NonInstancedObjectMock(std::shared_ptr<Shader> shaderPtr, std::shared_ptr<NonInstancedRasterizer> rasterizer, ObjectLocation objectLocation)
        : NonInstancedObject(shaderPtr, rasterizer, objectLocation) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, drawNormals, (const std::shared_ptr<Shader> shaderPtr, (std::array<float, MathUtils::MAT4_SIZE> projectionMatrix), (std::array<float, MathUtils::MAT4_SIZE> viewMatrix)), (override));
    MOCK_METHOD(void, updateLocation, (const MathUtils::Vec3& newPosition), (override));
};

class InstancedObjectMock : public InstancedObject 
{
public:
    InstancedObjectMock(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer)
        : InstancedObject(shaderPtr, rasterizer) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, drawNormals, (const std::shared_ptr<Shader> shaderPtr, (std::array<float, MathUtils::MAT4_SIZE> projectionMatrix), (std::array<float, MathUtils::MAT4_SIZE> viewMatrix)), (override));
    MOCK_METHOD(void, addInstancedObject, ((const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix), float textureID), (override));
    MOCK_METHOD(void, removeInstancedObject, (unsigned int index), (override));
};

class GeometricInstancedObjectMock : public GeometricInstancedObject 
{
public:
    GeometricInstancedObjectMock(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer, const ObjectMaterialProperties& objectMaterialProperties)
        : GeometricInstancedObject(shaderPtr, rasterizer, objectMaterialProperties) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
};

class GeometricNonInstancedObjectMock : public GeometricNonInstancedObject 
{
public:
    GeometricNonInstancedObjectMock(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<NonInstancedRasterizer> rasterizer, const ObjectLocation& objectLocation, const ObjectMaterialProperties& objectMaterialProperties)
        : GeometricNonInstancedObject(shaderPtr, rasterizer, objectLocation, objectMaterialProperties) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
};

class DirectionalLightMock : public DirectionalLight 
{
public:
    DirectionalLightMock(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const LightProperties& lightProperties, const PhongLightingParams& phongLightingParams)
        : DirectionalLight(shaderPtrs, lightProperties, phongLightingParams) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, setDirectionLightParams, (const LightProperties& lightProperties, const PhongLightingParams& phongLightingParams), (override));
};

class LightObjectMock : public LightObject 
{
public:
    LightObjectMock(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer, const PhongLightingParams& phongLightProperties)
        : LightObject(shaderPtr, rasterizer, phongLightProperties) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
};

class PointLightMock : public PointLight 
{
public:
    PointLightMock(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const std::shared_ptr<InstancedRasterizer> rasterizer, const ObjectMaterialProperties& objectMaterialProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams)
        : PointLight(shaderPtrs, rasterizer, objectMaterialProperties, phongLightProperties, attenuationParams) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, updateLightProperties, (), (override));
};

class SpotLightMock : public SpotLight 
{
public:
    SpotLightMock(const std::vector<std::shared_ptr<Shader>> shaderPtrs, const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams, const SpotlightCutoffParams& spotlightCutoffParams)
        : SpotLight(shaderPtrs, lightProperties, phongLightProperties, attenuationParams, spotlightCutoffParams) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, updateSpotlightParams, (const LightProperties& lightProperties, const PhongLightingParams& phongLightProperties, const AttenuationParams& attenuationParams, const SpotlightCutoffParams& spotlightCutoffParams), (override));
    MOCK_METHOD(void, updatePosition, (const LightProperties& lightProperties), (override));
};

class SkyboxObjectMock : public SkyboxObject 
{
public:
    SkyboxObjectMock(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<SkyboxRasterizer> skyboxRasterizerPtr, const unsigned int textureID)
        : SkyboxObject(shaderPtr, skyboxRasterizerPtr, textureID) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (std::array<float, MathUtils::MAT4_SIZE> viewMatrix)), (override));
};

class TextObjectMock : public TextObject 
{
public:
    TextObjectMock(const std::shared_ptr<Shader> shader, const std::shared_ptr<TextRasterizer> rasterizer, const TextProperties& textProperties)
        : TextObject(shader, rasterizer, textProperties) {}

    MOCK_METHOD(void, render, ((const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix), (const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)), (override));
    MOCK_METHOD(void, updateTextProperties, (const TextProperties& newTextProperties), (override));
};

#endif // RENDEROBJECTMOCK_HPP
