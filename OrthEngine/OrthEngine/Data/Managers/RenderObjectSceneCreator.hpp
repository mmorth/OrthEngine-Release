#ifndef RENDEROBJECTSCENECREATOR_HPP
#define RENDEROBJECTSCENECREATOR_HPP

#include <array>

#include "Factories/FactoryStructs.hpp"
#include "Transform/Transform.hpp"

enum class PlaneDimension
{
    X_AXIS = 1,
    Y_AXIS = 2,
    Z_AXIS = 3
};

struct AxisPlaneDrawInfo
{
    MathUtils::Vec2 axis1Range;
    MathUtils::Vec2 axis2Range;
    float fixedAxisPosition;

    // Default constructor
    AxisPlaneDrawInfo()
        : axis1Range(MathUtils::Vec2(0.0f, 0.0f)), axis2Range(MathUtils::Vec2(0.0f, 0.0f)), fixedAxisPosition(0.0f)
    {}

    // Parameterized constructor
    AxisPlaneDrawInfo(const MathUtils::Vec2& axis1Range, const MathUtils::Vec2& axis2Range, float fixedAxisPosition)
        : axis1Range(axis1Range), axis2Range(axis2Range), fixedAxisPosition(fixedAxisPosition)
    {}
};

class RenderObjectSceneCreator 
{
public:
    RenderObjectSceneCreator();
    ~RenderObjectSceneCreator();

    virtual std::vector<RenderObjectConfig> createExperimentalScene();
    virtual void drawInstancedPlane(const PlaneDimension& planeDimension, const AxisPlaneDrawInfo& axisPlaneDrawInfo, std::vector< std::array<float, MathUtils::MAT4_SIZE>>& modelMatVec, std::vector<float>& textureIDsVec);

private:
    unsigned int m_numInstancedObjects;

};

#endif // RENDEROBJECTSCENECREATOR_HPP