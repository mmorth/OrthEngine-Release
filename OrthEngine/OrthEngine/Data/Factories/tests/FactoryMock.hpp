#ifndef FACTORYMOCK_HPP
#define FACTORYMOCK_HPP

#include <gmock/gmock.h>

#include "Factories/CameraFactory.hpp"
#include "Factories/FramebufferFactory.hpp"
#include "Factories/RenderObjectFactory.hpp"
#include "Factories/ShaderFactory.hpp"
#include "Factories/VertexDataFactory.hpp"

class VertexDataFactoryMock : public VertexDataFactory
{
public:
    MOCK_METHOD(void, addVertexData, (const std::string& key, const VertexData& vertexData), (override));
    MOCK_METHOD(std::optional<VertexData>, getVertexData, (const std::string& key), (override));
};

class ShaderFactoryMock : public ShaderFactory
{
public:
    MOCK_METHOD(void, addShader, (const std::string& key, const ShaderFileNames& shader), (override));
    MOCK_METHOD(std::optional<std::shared_ptr<Shader>>, getShader, (const std::string& key), (override));
};

class CameraFactoryMock : public CameraFactory
{
public:
    MOCK_METHOD(std::unique_ptr<Camera>, getCamera, (const CameraTypes cameraTypes, const MathUtils::Vec3& position, const MathUtils::Vec3& front), (override));
};

class FramebufferFactoryMock : public FramebufferFactory 
{
public:
    MOCK_METHOD(void, initialize, (std::shared_ptr<ShaderFactory> shaderFactory, std::shared_ptr<VertexDataFactory> vertexDataFactory), (override));
    MOCK_METHOD(std::unique_ptr<Framebuffer>, getFramebuffer, (), (override));
};

class RasterizerFactoryMock : public RasterizerFactory 
{
public:
    MOCK_METHOD(std::optional<std::shared_ptr<Rasterizer>>, getRasterizer, (GeometryTypes geometryTypes), (override));
};

class RenderObjectFactoryMock : public RenderObjectFactory 
{
public:
    MOCK_METHOD(std::optional<std::unique_ptr<RenderObject>>, createRenderObject, (const RenderObjectConfig& renderObjectConfig), (override));
    MOCK_METHOD(void, initialize, (std::shared_ptr<ShaderFactory> shaderFactory, std::shared_ptr<RasterizerFactory> rasterizerFactory, std::shared_ptr<TextureFactory> textureFactory), (override));
};

#endif // FACTORYMOCK_HPP
