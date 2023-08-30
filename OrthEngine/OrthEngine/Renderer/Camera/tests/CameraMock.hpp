#ifndef CAMERAMOCK_HPP
#define CAMERAMOCK_HPP

#include <gmock/gmock.h>

#include "Camera/Camera.hpp"
#include "Camera/FirstPersonCamera.hpp"
#include "Camera/ThirdPersonCamera.hpp"


// ===============================================================
// CameraMock
// ===============================================================
class CameraMock : public Camera 
{
public:
    CameraMock(const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, const CameraAngles& cameraAngles = { 0.0f, -20.0f, 0.0f }, const CameraOptions& cameraOptions = { 2.5f, 0.5f, 45.0f })
        : Camera(position, front, cameraAngles, cameraOptions) {}

    MOCK_METHOD(void, ProcessKeyboard, (const CameraMovement& direction, float deltaTime), (override));
    MOCK_METHOD((std::array<float, MathUtils::MAT4_SIZE>), GetViewMatrix, (), (override));
    MOCK_METHOD(MathUtils::Vec3, GetFrontVector, (), (override));

    MOCK_METHOD(void, ProcessMouseMovement, (const float xOffset, const float yOffset, const GLboolean constrainPitch), (override));
    MOCK_METHOD(void, ProcessMouseScroll, (const float scrollAmount), (override));

    MOCK_METHOD(float, GetZoom, (), (override));
    MOCK_METHOD(MathUtils::Vec3, GetFront, (), (override));
    MOCK_METHOD(MathUtils::Vec3, GetPosition, (), (override));
    MOCK_METHOD(void, SetPosition, (MathUtils::Vec3 newCameraPosition), (override));
    MOCK_METHOD(void, SetTarget, (MathUtils::Vec3 newCameraTarget), (override));

protected:
    MOCK_METHOD(void, updateCameraVectors, (), (override));
    MOCK_METHOD((std::array<float, MathUtils::MAT4_SIZE>), computeViewMatrix, (const glm::mat4& viewMatrix), (override));
};


// ===============================================================
// FirstPersonCameraMock
// ===============================================================
class FirstPersonCameraMock : public CameraMock 
{
public:
    FirstPersonCameraMock(const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, const CameraAngles& cameraAngles = { 0.0f, -20.0f, 0.0f }, const CameraOptions& cameraOptions = { 2.5f, 0.5f, 45.0f })
        : CameraMock(position, front, cameraAngles, cameraOptions) {}

    MOCK_METHOD(void, ProcessKeyboard, (const CameraMovement& direction, const float deltaTime), (override));
    MOCK_METHOD((std::array<float, MathUtils::MAT4_SIZE>), GetViewMatrix, (), (override));
};


// ===============================================================
// ThirdPersonCameraMock
// ===============================================================
class ThirdPersonCameraMock : public CameraMock
{
public:
    ThirdPersonCameraMock(const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, const CameraAngles& cameraAngles = { 0.0f, -20.0f, 0.0f }, const CameraOptions& cameraOptions = { 2.5f, 0.5f, 45.0f })
        : CameraMock(position, front, cameraAngles, cameraOptions) {}

    MOCK_METHOD(void, ProcessKeyboard, (const CameraMovement& direction, const float deltaTime), (override));
    MOCK_METHOD((std::array<float, MathUtils::MAT4_SIZE>), GetViewMatrix, (), (override));
};

#endif // CAMERAMOCK_HPP
