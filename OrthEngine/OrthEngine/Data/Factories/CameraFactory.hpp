#ifndef CAMERAFACTORY_HPP
#define CAMERAFACTORY_HPP

#include <memory>
#include <optional>

#include "Camera/Camera.hpp"
#include "Camera/FirstPersonCamera.hpp"
#include "MathUtils.hpp"
#include "Camera/ThirdPersonCamera.hpp"

enum class CameraTypes 
{
    FIRST_PERSON = 1,
    THIRD_PERSON = 2
};

class CameraFactory 
{
public:
    static CameraFactory& getInstance();

    virtual std::unique_ptr<Camera> getCamera(const CameraTypes cameraTypes, const MathUtils::Vec3& position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, const MathUtils::Vec3& front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f });

private:
    CameraFactory() {};
};

#endif // CAMERAFACTORY_HPP
