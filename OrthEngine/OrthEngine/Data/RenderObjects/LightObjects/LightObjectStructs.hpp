#ifndef LIGHTOBJECTSTRUCTS_HPP
#define LIGHTOBJECTSTRUCTS_HPP

#include "MathUtils.hpp"

struct LightProperties
{
    MathUtils::Vec3 position;
    MathUtils::Vec3 direction;

    // Constructors
    LightProperties() : position(0.0f, 0.0f, 0.0f), direction(0.0f, 0.0f, 0.0f) {}
    LightProperties(const MathUtils::Vec3& position, const MathUtils::Vec3& direction)
        : position(position), direction(direction) {}

    bool operator==(const LightProperties& other) const
    {
        return position == other.position &&
            direction == other.direction;
    }
};

struct PhongLightingParams
{
    MathUtils::Vec3 ambient;
    MathUtils::Vec3 diffuse;
    MathUtils::Vec3 specular;

    // Constructors
    PhongLightingParams() : ambient(0.0f, 0.0f, 0.0f), diffuse(0.0f, 0.0f, 0.0f), specular(0.0f, 0.0f, 0.0f) {}
    PhongLightingParams(const MathUtils::Vec3& ambient, const MathUtils::Vec3& diffuse, const MathUtils::Vec3& specular)
        : ambient(ambient), diffuse(diffuse), specular(specular) {}

    bool operator==(const PhongLightingParams& other) const
    {
        return ambient == other.ambient &&
            diffuse == other.diffuse &&
            specular == other.specular;
    }
};

struct AttenuationParams
{
    float constant;
    float linear;
    float quadratic;

    // Constructors
    AttenuationParams() : constant(0.0f), linear(0.0f), quadratic(0.0f) {}
    AttenuationParams(float constant, float linear, float quadratic)
        : constant(constant), linear(linear), quadratic(quadratic) {}

    bool operator==(const AttenuationParams& other) const
    {
        return constant == other.constant &&
            linear == other.linear &&
            quadratic == other.quadratic;
    }
};

struct SpotlightCutoffParams
{
    float innerCutoff;
    float outerCutoff;

    // Constructors
    SpotlightCutoffParams() : innerCutoff(0.0f), outerCutoff(0.0f) {}
    SpotlightCutoffParams(float innerCutoff, float outerCutoff)
        : innerCutoff(innerCutoff), outerCutoff(outerCutoff) {}

    bool operator==(const SpotlightCutoffParams& other) const
    {
        return innerCutoff == other.innerCutoff &&
            outerCutoff == other.outerCutoff;
    }
};

#endif // LIGHTOBJECTSTRUCTS_HPP
