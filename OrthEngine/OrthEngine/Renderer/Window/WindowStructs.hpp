#ifndef WINDOWSTRUCTS_HPP
#define WINDOWSTRUCTS_HPP

#include "MathUtils.hpp"

struct WindowLayout
{
    MathUtils::Vec2 dimensions;
    std::string name;
    bool limitFPS;

    WindowLayout()
        : dimensions(800, 600), name("Window"), limitFPS(true)
    {
    }

    WindowLayout(MathUtils::Vec2 dimensions, std::string name, bool limitFPS)
        : dimensions(dimensions), name(name), limitFPS(limitFPS)
    {
    }
};

struct WindowHints
{
    int majorVersion;
    int minorVersion;
    int numSamples;
    bool testingError;

    WindowHints()
        : majorVersion(3), minorVersion(3), numSamples(4), testingError(false)
    {
    }

    WindowHints(int majorVersion, int minorVersion, int numSamples, bool testingError)
        : majorVersion(majorVersion), minorVersion(minorVersion), numSamples(numSamples), testingError(testingError)
    {
    }
};

struct WindowSettings
{
    WindowLayout windowLayout;
    WindowHints windowHints;

    WindowSettings()
        : windowLayout(), windowHints()
    {
    }

    WindowSettings(WindowLayout windowLayout, WindowHints windowHints)
        : windowLayout(windowLayout), windowHints(windowHints)
    {
    }
};

#endif // WINDOWSTRUCTS_HPP