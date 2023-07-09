#ifndef RASTERIZERFACTORY_HPP
#define RASTERIZERFACTORY_HPP

#include <optional>
#include <unordered_map>

#include "FactoryStructs.hpp"
#include "Rasterizers/InstancedRasterizer.hpp"
#include "Rasterizers/NonInstancedRasterizer.hpp"
#include "Rasterizers/SkyboxRasterizer.hpp"
#include "Rasterizers/TextRasterizer.hpp"
#include "Rasterizers/Rasterizer.hpp"
#include "Rasterizers/RasterizerUtility.hpp"

#include "VertexDataFactory.hpp"

class RasterizerFactory 
{
public:
    static RasterizerFactory& getInstance();

    void RasterizerFactory::setRasterizerDefault();
    virtual std::optional<std::shared_ptr<Rasterizer>> getRasterizer(const GeometryTypes geometryTypes);

protected:
    RasterizerFactory();

private:
    VertexDataFactory m_vertexDataFactory;

    std::shared_ptr<InstancedRasterizer> m_instancedCubeRasterizer;
    std::shared_ptr<NonInstancedRasterizer> m_nonInstancedCubeRasterizer;
    std::shared_ptr<InstancedRasterizer> m_instancedPlaneRasterizer;
    std::shared_ptr<NonInstancedRasterizer> m_nonInstancedPlaneRasterizer;
    std::shared_ptr<NonInstancedRasterizer> m_quadRasterizer;
    std::shared_ptr<SkyboxRasterizer> m_skyboxRasterizer;
    std::shared_ptr<TextRasterizer> m_textRasterizer;
    std::shared_ptr<InstancedRasterizer> m_pointLightRasterizer;
};

#endif  // RASTERIZERFACTORY_HPP