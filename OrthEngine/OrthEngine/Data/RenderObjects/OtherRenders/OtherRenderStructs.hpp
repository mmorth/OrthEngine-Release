#ifndef OTHERRENDERSTRUCTS_HPP
#define OTHERRENDERSTRUCTS_HPP

#include <string>

#include "MathUtils.hpp"

struct TextProperties 
{
    std::string text;
    MathUtils::Vec2 position;
    float scale;
    MathUtils::Vec3 color;

    // Constructors
    TextProperties() : text(""), position(0.0f, 0.0f), scale(0.0f), color(0.0f, 0.0f, 0.0f) {}
    TextProperties(const std::string& text, const MathUtils::Vec2& position, float scale, const MathUtils::Vec3& color)
        : text(text), position(position), scale(scale), color(color) {}

    bool operator==(const TextProperties& other) const
    {
        return text == other.text &&
            position == other.position &&
            scale == other.scale &&
            color == other.color;
    }
};

#endif // OTHERRENDERSTRUCTS_HPP
