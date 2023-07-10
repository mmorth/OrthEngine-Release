#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <array>
#include <algorithm>
#include <iostream>
#include <vector>

#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MathUtils.hpp"

enum class CameraMovement {
    NONE,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    ROLL_LEFT,
    ROLL_RIGHT
};

struct CameraAngles
{
    float yaw;
    float pitch;
    float roll;

    bool operator==(const CameraAngles& other) const
    {
        return std::abs(yaw - other.yaw) < MathUtils::EPSILON &&
            std::abs(pitch - other.pitch) < MathUtils::EPSILON &&
            std::abs(roll - other.roll) < MathUtils::EPSILON;
    }
};

struct CameraOptions
{
    float movementSpeed;
    float mouseSensitivity;
    float zoom;

    bool operator==(const CameraOptions& other) const
    {
        return std::abs(movementSpeed - other.movementSpeed) < MathUtils::EPSILON &&
            std::abs(mouseSensitivity - other.mouseSensitivity) < MathUtils::EPSILON &&
            std::abs(zoom - other.zoom) < MathUtils::EPSILON;
    }
};

struct CameraAttributes
{
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 target;

    bool operator==(const CameraAttributes& other) const
    {
        return glm::all(glm::epsilonEqual(position, other.position, MathUtils::EPSILON)) &&
            glm::all(glm::epsilonEqual(front, other.front, MathUtils::EPSILON)) &&
            glm::all(glm::epsilonEqual(up, other.up, MathUtils::EPSILON)) &&
            glm::all(glm::epsilonEqual(right, other.right, MathUtils::EPSILON)) &&
            glm::all(glm::epsilonEqual(target, other.target, MathUtils::EPSILON));
    }
};

struct WorldAxis
{
    glm::vec3 up;
    glm::vec3 front;
    glm::vec3 right;

    bool operator==(const WorldAxis& other) const
    {
        return glm::all(glm::epsilonEqual(up, other.up, MathUtils::EPSILON)) &&
            glm::all(glm::epsilonEqual(front, other.front, MathUtils::EPSILON)) &&
            glm::all(glm::epsilonEqual(right, other.right, MathUtils::EPSILON));
    }
};

class Camera
{
public:
    Camera(const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, const CameraAngles& cameraAngles = { 0.0f, -20.0f, 0.0f }, const CameraOptions& cameraOptions = { 2.5f, 0.5f, 45.0f });
    ~Camera();

    virtual void ProcessKeyboard(const CameraMovement& direction, const float deltaTime);
    virtual std::array<float, MathUtils::MAT4_SIZE> GetViewMatrix();
    virtual MathUtils::Vec3 GetFrontVector();

    virtual void ProcessMouseMovement(const float xOffset, const float yOffset, const GLboolean constrainPitch = true);
    virtual void ProcessMouseScroll(const float scrollAmount);

    virtual float GetZoom() { return m_cameraOptions.zoom; }
    virtual MathUtils::Vec3 GetFront() { return { m_cameraAttributes.front.x, m_cameraAttributes.front.y, m_cameraAttributes.front.z }; }
    virtual MathUtils::Vec3 GetPosition() { return { m_cameraAttributes.position.x, m_cameraAttributes.position.y, m_cameraAttributes.position.z }; }

protected:
    virtual void updateCameraVectors();
    virtual std::array<float, MathUtils::MAT4_SIZE> computeViewMatrix(const glm::mat4& viewMatrix);

    CameraAttributes m_cameraAttributes;
    WorldAxis m_worldAxis;
    CameraAngles m_cameraAngles;
    CameraOptions m_cameraOptions;

    // Constants
    static constexpr float CAM_PLAYER_DISTANCE = 4.0f;

    static constexpr float MIN_ZOOM = 1.0f;
    static constexpr float MAX_ZOOM = 45.0f;

    static constexpr float MIN_PITCH = -89.0f;
    static constexpr float MAX_PITCH = 89.0f;
};

#endif // CAMERA_HPP