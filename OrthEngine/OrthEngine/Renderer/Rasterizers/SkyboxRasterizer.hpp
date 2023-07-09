#ifndef SKYBOXRASTERIZER_HPP
#define SKYBOXRASTERIZER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Rasterizers/Rasterizer.hpp"

class SkyboxRasterizer : public Rasterizer 
{
public:
    SkyboxRasterizer(const VertexData& vertexProperties);
    ~SkyboxRasterizer();

    virtual unsigned int createNewVAO() override;
    virtual void drawArrays(const unsigned int VAO, const unsigned int numInstances = 1) override;

};

#endif // SKYBOXRASTERIZER_HPP
