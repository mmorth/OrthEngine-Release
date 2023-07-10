#include "RasterizerFactory.hpp"

// ------------------------------------------------------------------------
RasterizerFactory& RasterizerFactory::getInstance()
{
    static RasterizerFactory instance;
    return instance;
}

// ------------------------------------------------------------------------
RasterizerFactory::RasterizerFactory()
    : m_vertexDataFactory(VertexDataFactory::getInstance())
    , m_instancedCubeRasterizer(std::make_shared<InstancedRasterizer>(m_vertexDataFactory.getVertexData("Default_Cube").value(), std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{}))
    , m_nonInstancedCubeRasterizer(std::make_shared<NonInstancedRasterizer>(m_vertexDataFactory.getVertexData("Default_Cube").value()))
    , m_instancedPlaneRasterizer(std::make_shared<InstancedRasterizer>(m_vertexDataFactory.getVertexData("Default_Plane").value(), std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{}))
    , m_nonInstancedPlaneRasterizer(std::make_shared<NonInstancedRasterizer>(m_vertexDataFactory.getVertexData("Default_Plane").value()))
    , m_quadRasterizer(std::make_shared<NonInstancedRasterizer>(m_vertexDataFactory.getVertexData("Default_Cube").value()))
    , m_skyboxRasterizer(std::make_shared<SkyboxRasterizer>(m_vertexDataFactory.getVertexData("Default_Skybox").value()))
    , m_textRasterizer(std::make_shared<TextRasterizer>(m_vertexDataFactory.getVertexData("Default_Quad").value()))
    , m_pointLightRasterizer(std::make_shared<InstancedRasterizer>(m_vertexDataFactory.getVertexData("Default_Cube").value(), std::vector<std::array<float, MathUtils::MAT4_SIZE>>{}, std::vector<float>{}))
{
    LOG(INFO) << "ctor";

    setRasterizerDefault();
}

// ------------------------------------------------------------------------
void RasterizerFactory::setRasterizerDefault()
{
    //Enable depth, stencil, blending, culling, MSAA, and wireframing
    RasterizerUtility::setDepthTestState(GlobalSettings::getInstance().getValue<bool>("depthState", true));
    RasterizerUtility::setBlendState(GlobalSettings::getInstance().getValue<bool>("blendState", true));
    RasterizerUtility::enableFaceCulling();
    RasterizerUtility::setWireframeDrawingState(GlobalSettings::getInstance().getValue<bool>("wireframeState", false));
}

// ------------------------------------------------------------------------
std::optional<std::shared_ptr<Rasterizer>> RasterizerFactory::getRasterizer(const GeometryTypes geometryTypes)
{
    LOG(INFO) << "Rasterizer retrieved: " << static_cast<int>(geometryTypes);

    switch (geometryTypes)
    {
        case GeometryTypes::INSTANCED_CUBE:
            return m_instancedCubeRasterizer;
        case GeometryTypes::NONINSTANCED_CUBE:
            return m_nonInstancedCubeRasterizer;
        case GeometryTypes::INSTANCED_PLANE:
            return m_instancedPlaneRasterizer;
        case GeometryTypes::NONINSTANCED_PLANE:
            return m_nonInstancedPlaneRasterizer;
        case GeometryTypes::QUAD:
            return m_quadRasterizer;
        case GeometryTypes::SKYBOX:
            return m_skyboxRasterizer;
        case GeometryTypes::TEXT:
            return m_textRasterizer;
        case GeometryTypes::INSTANCED_POINT_LIGHT:
                return m_pointLightRasterizer;
        default:
            return std::nullopt;
    }
}
