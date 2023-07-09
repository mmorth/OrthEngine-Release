#include "ThirdPersonCamera.hpp"

// ------------------------------------------------------------------------
ThirdPersonCamera::ThirdPersonCamera(const MathUtils::Vec3& position, const MathUtils::Vec3& front, const CameraAngles& cameraAngles, const CameraOptions& cameraOptions)
    : Camera(position, front, cameraAngles, cameraOptions)
{
    //LOG(INFO) << "ctor";

    updateCameraVectors();
}

// ------------------------------------------------------------------------
ThirdPersonCamera::~ThirdPersonCamera()
{
    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
std::array<float, MathUtils::MAT4_SIZE> ThirdPersonCamera::GetViewMatrix()
{
    glm::mat4 viewMatrix = glm::lookAt(m_cameraAttributes.position, m_cameraAttributes.target, m_worldAxis.up);
    return computeViewMatrix(viewMatrix);
}

// ------------------------------------------------------------------------
void ThirdPersonCamera::ProcessKeyboard(const CameraMovement& direction, const float deltaTime)
{
    float velocity = m_cameraOptions.movementSpeed * deltaTime;

    switch (direction)
    {
        case CameraMovement::FORWARD:
            m_cameraAttributes.target += velocity * m_cameraAttributes.front;
            break;

        case CameraMovement::BACKWARD:
            m_cameraAttributes.target -= velocity * m_cameraAttributes.front;
            break;

        case CameraMovement::LEFT:
            m_cameraAttributes.target -= velocity * m_cameraAttributes.right;
            break;

        case CameraMovement::RIGHT:
            m_cameraAttributes.target += velocity * m_cameraAttributes.right;
            break;

        default:
            break;
    }

    Camera::ProcessKeyboard(direction, deltaTime);
}

// ------------------------------------------------------------------------
void ThirdPersonCamera::updateCameraVectors()
{
    Camera::updateCameraVectors();

    m_cameraAttributes.position = m_cameraAttributes.target - m_cameraAttributes.front * CAM_PLAYER_DISTANCE;
}