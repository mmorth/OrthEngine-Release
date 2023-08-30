#ifndef FACTORYSTRUCTS_HPP
#define FACTORYSTRUCTS_HPP

#include <array>
#include <string>
#include <vector>

#include "MathUtils.hpp"

#include "RenderObjects/LightObjects/LightObjectStructs.hpp"
#include "RenderObjects/OtherRenders/OtherRenderStructs.hpp"
#include "ObjectStructs.hpp"

// ===== ObjectConfig Structs =====

struct TransformationMatrices 
{
    std::array<float, MathUtils::MAT4_SIZE> projectionMatrix;
    std::array<float, MathUtils::MAT4_SIZE> viewMatrix;
    MathUtils::Vec3 camPosition;
};

// ===== ObjectConfig defs =====

enum class GeometryTypes 
{
    INSTANCED_CUBE = 1,
    NONINSTANCED_CUBE = 2,
    INSTANCED_PLANE = 3,
    NONINSTANCED_PLANE = 4,
    QUAD = 5,
    SKYBOX = 6,
    TEXT = 7,
    INSTANCED_POINT_LIGHT = 8,
    INSTANCED_SPOT_LIGHT = 9,
    DIRECTIONAL_LIGHT = 10,
};

struct RenderObjectProperties 
{
    GeometryTypes geometryType;
    bool isPlayer;
    bool isCollidable;
};

struct ObjectProperties 
{
    MathUtils::Vec3 position;
    MathUtils::Vec3 size;
    float orientation;
};

struct ObjectMaterialNames
{
    float shininess;
    std::string diffuseMap;
    std::string specularMap;
};

struct InstancedObjectProperties 
{
    std::vector<std::array<float, MathUtils::MAT4_SIZE>> modelMat;
    std::vector<float> textureIDs;

    std::string textureArrayName;
    std::vector<std::string> textureArrayNames;
};

struct ObjectConfig 
{
    // RenderObject Properties
    RenderObjectProperties renderObjectProperties;
    ObjectLocationOrientation objectLocationOrientation;
    ObjectMaterialNames objectMaterialNames;
    InstancedObjectProperties instancedObjectProperties;

    // LightObject Properties
    LightProperties lightProperties;
    PhongLightingParams phongLightProperties;
    AttenuationParams attenuationParams;
    SpotlightCutoffParams spotlightCutoffParams;

    // Other Properties
    TextProperties textProperties;
    PhysicsShapeInfo physicsShapeInfo;
};

#endif // FACTORYSTRUCTS_HPP
