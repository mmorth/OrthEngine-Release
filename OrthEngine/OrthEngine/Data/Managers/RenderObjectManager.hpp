#ifndef RENDEROBJECTMANAGER_HPP
#define RENDEROBJECTMANAGER_HPP

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>

#include "Factories/FramebufferFactory.hpp"
#include "Factories/RenderObjectFactory.hpp"
#include "Factories/ShaderFactory.hpp"
#include "GlobalSettingsConstants/GlobalSettings.hpp"
#include "Rasterizers/RasterizerUtility.hpp"
#include "RenderObjectSceneCreator.hpp"

#include "Window/Window.hpp"

struct RenderOptions 
{
    bool drawNormals;
    bool framebufferEnabled;
    bool msaaEnabled;
    bool drawOutlines;
};

class RenderObjectManager 
{
public:
    RenderObjectManager();
    ~RenderObjectManager();

    virtual void initialize(const std::shared_ptr<FramebufferFactory> framebufferFactory = nullptr, const std::shared_ptr<RenderObjectFactory> renderObjectFactory = nullptr, const std::shared_ptr<ShaderFactory> shaderFactory = nullptr, const std::shared_ptr<RenderObjectSceneCreator> renderObjectSceneCreator = nullptr);

    // RenderObject creation functions
    virtual RenderObject* createRenderObject(const unsigned int curID, const ObjectConfig& renderObjectConfig);

    // RenderObject update functions
    virtual void updateFpsText(const std::string& newFpsString);
    virtual bool updateRenderObject(const RenderObjectProperties& renderObjectProperties, const ObjectConfig& renderObjectConfig);
    virtual void removeObjectWithID(const unsigned int objectID);

    virtual void createInstancedObject(RenderObject* renderObjectPtr, const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix, float textureID);
    virtual bool removeInstancedObjectWithID(const unsigned int renderObjectID, const unsigned int instancedID);

    virtual void updatePointLight(RenderObject* renderObjectPtr);
    virtual void updateFramebufferSize(const int screenWidth, const int screenHeight);

    // render functions
    virtual TransformationMatrices getTransformationMatrices(const std::shared_ptr<Camera>& camera, const int windowWidth, const int windowHeight);
    virtual void renderAll(TransformationMatrices& transformationMatrices, const RenderLoopConfigOptions& renderLoopConfigOptions);

    // render loop functions
    virtual void renderLoopSetup(const SetupTeardownSettings& setupTeardownSettings);
    virtual void renderLoopCleanup(const TransformationMatrices& transformationMatrices, const SetupTeardownSettings& setupTeardownSettings);

    virtual std::vector<unsigned char> getFramebufferContent();

protected:
    std::shared_ptr<FramebufferFactory> m_framebufferFactory;
    std::shared_ptr<RenderObjectFactory> m_renderObjectFactory;
    std::shared_ptr<ShaderFactory> m_shaderFactory;
    std::shared_ptr<RenderObjectSceneCreator> m_renderObjectSceneCreator;

    std::unique_ptr<Framebuffer> m_framebuffer;
    std::unique_ptr<SkyboxObject> m_skyboxObject;
    std::unordered_map<int, std::unique_ptr<RenderObject>> m_renderObjects;
    std::vector<unsigned int> m_playerIDs;    
    
    unsigned int m_fpsTextID;

    static constexpr unsigned int RESERVED_IDS = 2;

private:
    std::vector<unsigned char> m_testFramebufferContents;

};

#endif // RENDEROBJECTMANAGER_HPP