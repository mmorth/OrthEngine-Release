#ifndef THIRDPERSONCAMERA_HPP
#define THIRDPERSONCAMERA_HPP

#include "Camera.hpp"

class ThirdPersonCamera : public Camera
{
public:
    ThirdPersonCamera(const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, const CameraAngles& cameraAngles = { 0.0f, -20.0f, 0.0f }, const CameraOptions& cameraOptions = { 2.5f, 0.5f, 45.0f });
    ~ThirdPersonCamera();

    virtual void ProcessKeyboard(const CameraMovement& direction, const float deltaTime) override;
    virtual std::array<float, MathUtils::MAT4_SIZE> GetViewMatrix() override;

protected:
    void updateCameraVectors() override;
};

#endif // THIRDPERSONCAMERA_HPP
