#include "CameraFactory.hpp"

// ------------------------------------------------------------------------
CameraFactory& CameraFactory::getInstance()
{
    static CameraFactory instance;
    return instance;
}

// ------------------------------------------------------------------------
std::unique_ptr<Camera> CameraFactory::getCamera(const CameraTypes cameraTypes, const MathUtils::Vec3& position, const MathUtils::Vec3& front)
{
    LOG(INFO) << "Camera created: " << static_cast<int>(cameraTypes);

    switch (cameraTypes)
    {
    case CameraTypes::FIRST_PERSON:
        return std::make_unique<FirstPersonCamera>(position, front);
    case CameraTypes::THIRD_PERSON:
        return std::make_unique<ThirdPersonCamera>(position, front);
    default:
        return std::make_unique<FirstPersonCamera>(position, front);
    }
}