#include "Camera.hpp"

// ------------------------------------------------------------------------
Camera::Camera(const MathUtils::Vec3& position, const MathUtils::Vec3& front, const CameraAngles& cameraAngles, const CameraOptions& cameraOptions)
    : m_cameraAttributes{ glm::vec3(position.x, position.y, position.z), glm::vec3(front.x, front.y, front.z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) }
    , m_worldAxis{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f) }
    , m_cameraAngles(cameraAngles)
    , m_cameraOptions(cameraOptions)
{
    LOG(INFO) << "ctor";
    
    // Update camera vectors
    ProcessMouseMovement(0.0f, 0.0f, true);
    ProcessMouseScroll(0.0f);
    updateCameraVectors();
}

// ------------------------------------------------------------------------
Camera::~Camera()
{
    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
std::array<float, MathUtils::MAT4_SIZE> Camera::GetViewMatrix()
{
    glm::mat4 viewMatrix(0.0f);

    return computeViewMatrix(viewMatrix);
}

// ------------------------------------------------------------------------
std::array<float, MathUtils::MAT4_SIZE> Camera::computeViewMatrix(const glm::mat4& viewMatrix)
{
    std::array<float, MathUtils::MAT4_SIZE> viewMatrixArray;
    memcpy(viewMatrixArray.data(), glm::value_ptr(viewMatrix), sizeof(float) * MathUtils::MAT4_SIZE);

    return viewMatrixArray;
}

// ------------------------------------------------------------------------
MathUtils::Vec3 Camera::GetFrontVector()
{
    glm::vec3 posFront = m_cameraAttributes.position + (CAM_PLAYER_DISTANCE * m_cameraAttributes.front);
    
    return MathUtils::Vec3{ posFront.x, posFront.y, posFront.z };
}

// ------------------------------------------------------------------------
void Camera::ProcessKeyboard(const CameraMovement& direction, const float deltaTime)
{
    updateCameraVectors();
}

// ------------------------------------------------------------------------
void Camera::ProcessMouseMovement(const float xOffset, const float yOffset, const GLboolean constrainPitch)
{
    // Update the yaw and pitch values based on mouse sensitivity and x/y offset
    m_cameraAngles.yaw -= m_cameraOptions.mouseSensitivity * xOffset;
    m_cameraAngles.pitch += m_cameraOptions.mouseSensitivity * yOffset;

    // Clamp pitch to be between -90 and 90 degrees to avoid sudden camera jumps
    if (constrainPitch)
        m_cameraAngles.pitch = std::clamp(m_cameraAngles.pitch, MIN_PITCH, MAX_PITCH);

    // Update camera attribute vectors based on new Euler angles
    updateCameraVectors();
}

// -------------------------------------------------------------------------
void Camera::ProcessMouseScroll(const float scrollAmount)
{
    m_cameraOptions.zoom -= scrollAmount;
    m_cameraOptions.zoom = std::clamp(m_cameraOptions.zoom, MIN_ZOOM, MAX_ZOOM);
}

// ------------------------------------------------------------------------
void Camera::updateCameraVectors()
{
    glm::quat roll = glm::angleAxis(glm::radians(m_cameraAngles.roll), m_worldAxis.front);
    glm::quat pitch = glm::angleAxis(glm::radians(m_cameraAngles.pitch), m_worldAxis.right);
    glm::quat yaw = glm::angleAxis(glm::radians(m_cameraAngles.yaw), m_worldAxis.up);

    // Apply roll first to enable yaw and pitch to be based on the new roll coordinate systems
    glm::quat orientation = roll * yaw * pitch;

    // Compute the new front, right, and up camera vectors
    m_cameraAttributes.front = glm::normalize(orientation * m_worldAxis.front);
    m_cameraAttributes.right = glm::normalize(orientation * m_worldAxis.right);
    m_cameraAttributes.up = glm::normalize(orientation * m_worldAxis.up);
}
