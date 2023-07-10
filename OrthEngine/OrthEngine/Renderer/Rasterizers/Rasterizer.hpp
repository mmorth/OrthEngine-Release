#ifndef RASTERIZER_HPP
#define RASTERIZER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "DataStructs.hpp"

struct TextureMaps 
{
    unsigned int diffuseMap;
    unsigned int specularMap;

    bool operator==(const TextureMaps& other) const
    {
        return diffuseMap == other.diffuseMap &&
            specularMap == other.specularMap;
    }
};

class Rasterizer 
{
public:
    Rasterizer(const VertexData& vertexProperties);
    Rasterizer() : m_numVertices(0), m_VAOs(), m_attribVBO(0) {};
    ~Rasterizer();

    virtual unsigned int createNewVAO();
    virtual void drawArrays(const unsigned int VAO, const unsigned int numInstances = 1);

    virtual void activateTextures(const TextureMaps& textureMaps);
    virtual unsigned int getNumVertices();

protected:
    virtual void activateTexture(const unsigned int textureID);

    unsigned int m_numVertices;

    std::vector<unsigned int> m_VAOs;
    unsigned int m_attribVBO;
};

#endif // RASTERIZER_HPP
