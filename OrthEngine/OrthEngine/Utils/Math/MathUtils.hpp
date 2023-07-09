/**
 * @file MathUtils.hpp
 * @brief Declaration of the MathUtils namespace and related structs.
 */

#ifndef MATH_UTILS_HPP
#define MATH_UTILS_HPP

#include <cmath>

namespace MathUtils
{
    constexpr unsigned int MAT4_SIZE = 16; /**< Size of the transformation matrices [4x4] matrix. */
    constexpr unsigned int VEC4_SIZE = 4; /**< Size of a [4x1] vector. */
    constexpr unsigned int VEC3_SIZE = 3; /**< Size of a [3x1] vector. */
    constexpr float EPSILON = 1e-6f; // Adjust the epsilon value based on your tolerance requirement

    /**
     * @brief 2D vector struct
     */
    struct Vec2
    {
        float x; /**< X component of the vector */
        float y; /**< Y component of the vector */

        // Constructors
        Vec2() : x(0.0f), y(0.0f) {}
        Vec2(float x, float y) : x(x), y(y) {}

        // Comparison operators
        bool operator==(const Vec2& other) const 
        {
            return std::fabs(x - other.x) < EPSILON &&
                std::fabs(y - other.y) < EPSILON;
        }
    };

    /**
     * @brief 3D vector struct
     */
    struct Vec3 {
        float x; /**< X component of the vector */
        float y; /**< Y component of the vector */
        float z; /**< Z component of the vector */

        // Constructors
        Vec3() : x(0.0f), y(0.0f), z(0.0f) {}
        Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

        // Length calculation
        float length() const 
        {
            return std::sqrt(x * x + y * y + z * z);
        }

        // Scalar multiplication
        Vec3 operator*(float scalar) const
        {
            return { x * scalar, y * scalar, z * scalar };
        }

        // Vector addition
        Vec3 operator+(const Vec3& other) const 
        {
            return Vec3{ x + other.x, y + other.y, z + other.z };
        }

        // Comparison operators
        bool operator==(const Vec3& other) const 
        {
            return std::fabs(x - other.x) < EPSILON &&
                std::fabs(y - other.y) < EPSILON &&
                std::fabs(z - other.z) < EPSILON;
        }

        bool operator!=(const Vec3& other) const {
            return !(*this == other);
        }
    };

    /**
     * @brief 4D vector struct
     */
    struct Vec4
    {
        float x; /**< X component of the vector */
        float y; /**< Y component of the vector */
        float z; /**< Z component of the vector */
        float w; /**< W component of the vector */
    };
}

#endif // MATH_UTILS_HPP