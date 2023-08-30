/**
 * @file ObjectStructs.hpp
 * @brief Declaration of the ObjectUtils namespace and structs that are shared between RenderObject and RigidBody
 */

#ifndef OBJECTSTRUCTS_HPP
#define OBJECTSTRUCTS_HPP

#include <cmath>

#include "MathUtils.hpp"

enum class CollisionType
{
    CONTACT_STOP = 0
};

enum class ShapeType {
    Box
};

struct PhysicsProperties
{
    float mass;
    float friction;

    // Constructors
    PhysicsProperties()
        : mass(1.0f), friction(0.5f) {}
    PhysicsProperties(const float m, const float f)
        : mass(m), friction(f) {}
};

struct PhysicsShapeInfo
{
    ShapeType shapeType;
    PhysicsProperties physicsProperties;

    // Constructors
    PhysicsShapeInfo()
        : shapeType(ShapeType::Box), physicsProperties(0.0f, 1.0f) {}
    PhysicsShapeInfo(const ShapeType& type, const PhysicsProperties& physicsProperties)
        : shapeType(type), physicsProperties(physicsProperties) {}
};
    
struct ObjectLocationOrientation
{
    MathUtils::Vec3 position;
    MathUtils::Vec3 scale;
    MathUtils::Vec3 rotationAxis;
    float orientation;

    // Constructors
    ObjectLocationOrientation() 
        : position(0.0f, 0.0f, 0.0f), scale(0.0f, 0.0f, 0.0f), rotationAxis(0.0f, 0.0f, 0.0f), orientation(0.0f) {}
    ObjectLocationOrientation(MathUtils::Vec3 position, MathUtils::Vec3 scale, MathUtils::Vec3 rotationAxis, float orientation) 
        : position(position), scale(scale), rotationAxis(rotationAxis), orientation(orientation) {}

    bool operator==(const ObjectLocationOrientation& other) const
    {
        return position == other.position &&
            scale == other.scale &&
            rotationAxis == other.rotationAxis &&
            orientation == other.orientation;
    }
};

#endif // OBJECTSTRUCTS_HPP