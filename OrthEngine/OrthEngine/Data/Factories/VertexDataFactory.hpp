#ifndef VERTEXDATAFACTORY_HPP
#define VERTEXDATAFACTORY_HPP

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>

#include "Rasterizers/DataStructs.hpp"
#include "FactoryStructs.hpp"
#include "MathUtils.hpp"

class VertexDataFactory
{
public:
    static VertexDataFactory& getInstance();

    virtual void addVertexData(const std::string& key, const VertexData& vertexData);
    virtual std::optional<VertexData> getVertexData(const std::string& key);

protected:
    VertexDataFactory();

private:
    static VertexDataFactory instance;

    std::unordered_map<std::string, VertexData> m_vertexMap;
};

#endif // VERTEXDATAFACTORY_HPP
