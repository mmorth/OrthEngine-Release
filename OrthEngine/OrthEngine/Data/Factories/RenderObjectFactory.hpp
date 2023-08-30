#ifndef RENDEROBJECTFACTORY_HPP
#define RENDEROBJECTFACTORY_HPP

#include <unordered_set>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>

#include "Camera/Camera.hpp"
#include "FactoryStructs.hpp"
#include "ShaderFactory.hpp"
#include "RasterizerFactory.hpp"
#include "Texture/TextureFactory.hpp"

#include "RenderObjects/GeometricObjects/GeometricInstancedObject.hpp"
#include "RenderObjects/GeometricObjects/GeometricNonInstancedObject.hpp"
#include "RenderObjects/LightObjects/LightObject.hpp"
#include "RenderObjects/LightObjects/LightObjectStructs.hpp"
#include "RenderObjects/InstancedObject.hpp"
#include "RenderObjects/NonInstancedObject.hpp"
#include "RenderObjects/LightObjects/DirectionalLight.hpp"
#include "RenderObjects/LightObjects/PointLight.hpp"
#include "RenderObjects/LightObjects/SpotLight.hpp"
#include "RenderObjects/OtherRenders/SkyboxObject.hpp"
#include "RenderObjects/OtherRenders/TextObject.hpp"
#include "RenderObjects/RenderObject.hpp"

struct SetupTeardownSettings
{
    bool framebufferEnabled;
    bool msaaEnabled;
    bool drawNormals;
    bool isTestFramebuffer;

    // Default constructor
    SetupTeardownSettings()
        : framebufferEnabled(false), msaaEnabled(false), drawNormals(false), isTestFramebuffer(false)
    {}

    // All-parameter constructor
    SetupTeardownSettings(bool framebuffer, bool msaa, bool draw, bool isTest)
        : framebufferEnabled(framebuffer),  msaaEnabled(msaa), drawNormals(draw), isTestFramebuffer(isTest)
    {}
};

struct RenderLoopConfigOptions 
{
    LightProperties lightProperties;
    ObjectLocationOrientation objectLocation;
    SetupTeardownSettings setupTeardownSettings;
};

class RenderObjectFactory 
{
public:
    static RenderObjectFactory& getInstance();

    // object creation functions
    virtual std::optional<std::unique_ptr<RenderObject>> createRenderObject(const ObjectConfig& renderObjectConfig);
    virtual void initialize(const std::shared_ptr<ShaderFactory> shaderFactory, const std::shared_ptr<RasterizerFactory> rasterizerFactory, const std::shared_ptr<TextureFactory> textureFactory);

protected:
    RenderObjectFactory();

private:
    std::shared_ptr<ShaderFactory> m_shaderFactory;
    std::shared_ptr<RasterizerFactory> m_rasterizerFactory;
    std::shared_ptr<TextureFactory> m_textureFactory;

};

#endif // RENDEROBJECTFACTORY_HPP
