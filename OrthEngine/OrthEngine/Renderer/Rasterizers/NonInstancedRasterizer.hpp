#ifndef NONINSTANCEDRASTERIZER_HPP
#define NONINSTANCEDRASTERIZER_HPP

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Rasterizer.hpp"

class NonInstancedRasterizer : public Rasterizer 
{
public:
    NonInstancedRasterizer(const VertexData& vertexProperties);
    ~NonInstancedRasterizer();

    virtual unsigned int createNewVAO() override;
    virtual void drawArrays(const unsigned int VAO, const unsigned int numInstances = 1) override;
};

#endif // NONINSTANCEDRASTERIZER_HPP
