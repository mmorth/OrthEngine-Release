#ifndef RENDEROBJECTSTRUCTS_HPP
#define RENDEROBJECTSTRUCTS_HPP

#include "MathUtils.hpp"

struct ObjectLocation 
{
    MathUtils::Vec3 position;
    MathUtils::Vec3 size;
    float orientation;

    // Constructors
    ObjectLocation() : position(0.0f, 0.0f, 0.0f), size(0.0f, 0.0f, 0.0f), orientation(0.0f) {}
    ObjectLocation(MathUtils::Vec3 position, MathUtils::Vec3 size, float orientation) : position(position), size(size), orientation(orientation) {}

    bool operator==(const ObjectLocation& other) const
    {
        return position == other.position &&
            size == other.size &&
            orientation == other.orientation;
    }
};

#endif // RENDEROBJECTSTRUCTS_HPP
