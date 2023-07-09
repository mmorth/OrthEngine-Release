#ifndef GEOMETRICOBJECTSTRUCTS_HPP
#define GEOMETRICOBJECTSTRUCTS_HPP

#include "MathUtils.hpp"
#include "Texture/TextureFactory.hpp"

struct ObjectMaterialProperties 
{
    float shininess;
    unsigned int diffuseMap;
    unsigned int specularMap;

    // Constructors
    ObjectMaterialProperties() : shininess(0.0f), diffuseMap(0), specularMap(0) {}
    ObjectMaterialProperties(float shininess, unsigned int diffuseMap, unsigned int specularMap) : shininess(shininess), diffuseMap(diffuseMap), specularMap(specularMap) {}

    bool operator==(const ObjectMaterialProperties& other) const
    {
        return shininess == other.shininess &&
            diffuseMap == other.diffuseMap &&
            specularMap == other.specularMap;
    }
};

#endif // GEOMETRICOBJECTSTRUCTS_HPP
