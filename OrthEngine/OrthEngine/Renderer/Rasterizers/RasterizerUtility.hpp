#ifndef RASTERIZERUTILITY_HPP
#define RASTERIZERUTILITY_HPP

#include <iostream>
#include <string>
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum class DepthMethods
{
    DEPTH_LESS = 1,
    DEPTH_LEQUAL = 2
};

class RasterizerUtility 
{
public:
    static void setDepthTestState(const bool isEnabled);
    static void setDepthMethod(const DepthMethods depthMethod);
    static void setBlendState(const bool blendState);
    static void setSupersampleState(const bool supersampleState);
    static void enableFaceCulling();
    static void setWireframeDrawingState(const bool drawWireframe);
};

#endif // RASTERIZERUTILITY_HPP
