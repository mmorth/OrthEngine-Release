#include "FirstPersonCamera.hpp"

// ------------------------------------------------------------------------
FirstPersonCamera::FirstPersonCamera(const MathUtils::Vec3& position, const MathUtils::Vec3& front, const CameraAngles& cameraAngles, const CameraOptions& cameraOptions)
    : Camera(position, front, cameraAngles, cameraOptions)
{
    //LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
FirstPersonCamera::~FirstPersonCamera()
{
    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
std::array<float, MathUtils::MAT4_SIZE> FirstPersonCamera::GetViewMatrix()
{
    glm::mat4 viewMatrix = glm::lookAt(m_cameraAttributes.position, m_cameraAttributes.position + m_cameraAttributes.front, m_cameraAttributes.up);
    return computeViewMatrix(viewMatrix);
}

// ------------------------------------------------------------------------
void FirstPersonCamera::ProcessKeyboard(const CameraMovement& direction, const float deltaTime)
{
    float velocity = m_cameraOptions.movementSpeed * deltaTime;

    switch (direction)
    {
        case CameraMovement::FORWARD:
            m_cameraAttributes.position += velocity * m_cameraAttributes.front;
            break;

        case CameraMovement::BACKWARD:
            m_cameraAttributes.position -= velocity * m_cameraAttributes.front;
            break;

        case CameraMovement::LEFT:
            m_cameraAttributes.position -= velocity * m_cameraAttributes.right;
            break;

        case CameraMovement::RIGHT:
            m_cameraAttributes.position += velocity * m_cameraAttributes.right;
            break;

        case CameraMovement::ROLL_LEFT:
            m_cameraAngles.roll += velocity * 25;
            break;

        case CameraMovement::ROLL_RIGHT:
            m_cameraAngles.roll -= velocity * 25;
            break;

        default:
            break;
    }

    Camera::ProcessKeyboard(direction, deltaTime);
}
