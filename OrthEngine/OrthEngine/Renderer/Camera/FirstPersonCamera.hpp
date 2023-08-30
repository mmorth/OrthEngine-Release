#ifndef FIRSTPERSONCAMERA_HPP
#define FIRSTPERSONCAMERA_HPP

#include "Camera.hpp"

class FirstPersonCamera : public Camera
{
public:
    FirstPersonCamera(const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, const CameraAngles& cameraAngles = { 0.0f, -20.0f, 0.0f }, const CameraOptions& cameraOptions = { 2.5f, 0.5f, 45.0f });
    ~FirstPersonCamera();

    virtual void ProcessKeyboard(const CameraMovement& direction, const float deltaTime) override;
    virtual std::array<float, MathUtils::MAT4_SIZE> GetViewMatrix() override;
    virtual void SetTarget(MathUtils::Vec3 newCameraTarget) override;

};

#endif // FIRSTPERSONCAMERA_HPP