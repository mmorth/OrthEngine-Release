#include <gtest/gtest.h>

#include "OpenGLFixture.hpp"
#include "Camera/Camera.hpp"
#include "Camera/FirstPersonCamera.hpp"
#include "Camera/ThirdPersonCamera.hpp"

// ===============================================================
// CameraTestable
// ===============================================================
class CameraTestable : public Camera {
public:
    CameraTestable(MathUtils::Vec3 position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3 front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, CameraAngles cameraAngles = { 0.0f, -20.0f, 0.0f }, CameraOptions cameraOptions = { 2.5f, 0.5f, 45.0f })
        : Camera(position, front, cameraAngles, cameraOptions) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    CameraAttributes getCameraAttribues() { return m_cameraAttributes; }
    WorldAxis getWorldAxis() { return m_worldAxis; }
    CameraAngles getCameraAngles() { return m_cameraAngles; }
    CameraOptions getCameraOptions() { return m_cameraOptions; }

    float getCamPlayerDistance() { return Camera::CAM_PLAYER_DISTANCE; }
    float getMinZoom() { return Camera::MIN_ZOOM; }
    float getMaxZoom() { return Camera::MAX_ZOOM; }
    float getMinPitch() { return Camera::MIN_PITCH; }
    float getMaxPitch() { return Camera::MAX_PITCH; }
};

// ------------------------------------------------------------------------
class Cameratest : public OpenGLTestFixture
{
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// ------------------------------------------------------------------------
TEST_F(Cameratest, CreateDefaultCameraCorrectlySetsCameraProperties)
{
    CameraTestable cameraTestable;

    // Ensure correct default values applied
    CameraAttributes expectedCameraAttributes{ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.342020154f, -0.939692736f), glm::vec3(0.0f, 0.939692736f, -0.342020154f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f) };
    CameraAttributes actualCamAttribs = cameraTestable.getCameraAttribues();
    EXPECT_EQ(expectedCameraAttributes, cameraTestable.getCameraAttribues());
    WorldAxis expectedWorldAxis{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
    EXPECT_EQ(expectedWorldAxis, cameraTestable.getWorldAxis());
    CameraAngles expectedCameraAngles = { 0.0f, -20.0f, 0.0f };
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());
    CameraOptions expectedCameraOptions = { 2.5f, 0.5f, 45.0f };
    EXPECT_EQ(expectedCameraOptions, cameraTestable.getCameraOptions());

    // Getters return proper values
    EXPECT_EQ(MathUtils::Vec3(0.0f, 0.0f, 0.0f), cameraTestable.GetPosition());
    MathUtils::Vec3 expectedFront{ 0.0f, -0.342020154f, -0.939692736f };
    EXPECT_EQ(expectedFront, cameraTestable.GetFront());
    EXPECT_EQ(45.0f, cameraTestable.GetZoom());
    EXPECT_EQ(expectedFront * cameraTestable.getCamPlayerDistance(), cameraTestable.GetFrontVector());
}

// ------------------------------------------------------------------------
TEST_F(Cameratest, CreateCameraWithParametersCorrectlySetsCameraProperties)
{
    MathUtils::Vec3 expectedPosition{ 1.0f, 1.0f, 1.0f };
    MathUtils::Vec3 expectedFront{ 2.0f, 2.0f, 2.0f };
    CameraAngles expectedCameraAngles = { 3.0f, 3.0f, 3.0f };
    CameraOptions expectedCameraOptions = { 4.0f, 4.0f, 4.0f };
    CameraTestable cameraTestable(expectedPosition, expectedFront, expectedCameraAngles, expectedCameraOptions);

    // Ensure correct camera attributes sets based on constructor
    CameraAttributes expectedCameraAttributes{ glm::vec3(expectedPosition.x, expectedPosition.y, expectedPosition.z), glm::vec3(-0.0494535640f, 0.0549995415f, -0.997261047f), glm::vec3(0.0549995415f, 0.997117698f, 0.0522642396f), glm::vec3(0.997260928f, -0.0522642322f, -0.0523359627f), glm::vec3(0.0f, 0.0f, 0.0f) };
    CameraAttributes actualCamAttribs = cameraTestable.getCameraAttribues();
    EXPECT_EQ(expectedCameraAttributes, cameraTestable.getCameraAttribues());
    WorldAxis expectedWorldAxis{ glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.0f, 0.0f) };
    EXPECT_EQ(expectedWorldAxis, cameraTestable.getWorldAxis());
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());
    EXPECT_EQ(expectedCameraOptions, cameraTestable.getCameraOptions());

    // Getters return proper values
    EXPECT_EQ(expectedPosition, cameraTestable.GetPosition());
    MathUtils::Vec3 expectedFrontVec3{ -0.0494535640f, 0.0549995415f, -0.997261047f };
    EXPECT_EQ(expectedFrontVec3, cameraTestable.GetFront());
    EXPECT_EQ(expectedCameraOptions.zoom, cameraTestable.GetZoom());
    MathUtils::Vec3 expectedFrontVec = expectedPosition + expectedFrontVec3 * cameraTestable.getCamPlayerDistance();
    MathUtils::Vec3 actualFrontVec = cameraTestable.GetFrontVector();
    EXPECT_EQ(expectedFrontVec, cameraTestable.GetFrontVector());
}

// ------------------------------------------------------------------------
TEST_F(Cameratest, CreateCameraWithOutOfBoundsPitchAndScrollClampsActualValue)
{
    CameraAngles outOfRangePitch = { 180.0f, 180.0f, 180.0f };
    CameraOptions outOfRangeScroll = { 90.0f, 90.0f, 90.0f };
    CameraTestable cameraTestable({ 1.0f, 1.0f, 1.0f }, { 2.0f, 2.0f, 2.0f }, outOfRangePitch, outOfRangeScroll);

    // Ensure pitch and scroll clamped to valid values
    CameraAngles expectedCameraAngles = { outOfRangePitch.yaw, cameraTestable.getMaxPitch(), outOfRangePitch.roll };
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());
    CameraOptions expectedCameraOptions = { outOfRangeScroll.mouseSensitivity, outOfRangeScroll.movementSpeed, cameraTestable.getMaxZoom() };
    EXPECT_EQ(expectedCameraOptions, cameraTestable.getCameraOptions());
}

// ------------------------------------------------------------------------
TEST_F(Cameratest, ProcessMouseMovementsCorrectlyUpdatesYawAndPitchValues)
{
    CameraAngles ctorCameraAngles = { 1.0f, 1.0f, 1.0f };
    CameraOptions ctorCameraOptions = { 1.0f, 1.0f, 1.0f };
    CameraTestable cameraTestable({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, ctorCameraAngles, ctorCameraOptions);

    // Value mouse movement value
    float offset = 10.f;
    CameraAngles expectedCameraAngles = { ctorCameraAngles.yaw - offset, ctorCameraAngles.pitch + offset, ctorCameraAngles.roll };
    cameraTestable.ProcessMouseMovement(offset, offset, true);
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());

    // Above range Pitch should be clipped to max value
    offset = cameraTestable.getMaxPitch();
    expectedCameraAngles = { cameraTestable.getCameraAngles().yaw - offset, cameraTestable.getMaxPitch(), ctorCameraAngles.roll };
    cameraTestable.ProcessMouseMovement(offset, offset, true);
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());

    // Below range Pitch should be clipped to min value
    offset = 2 * cameraTestable.getMinPitch();
    expectedCameraAngles = { cameraTestable.getCameraAngles().yaw - offset, cameraTestable.getMinPitch(), ctorCameraAngles.roll };
    cameraTestable.ProcessMouseMovement(offset, offset, true);
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());

    // Pitch allowed to go out of range when pitch not constrained
    offset = cameraTestable.getMinPitch();
    expectedCameraAngles = { cameraTestable.getCameraAngles().yaw - offset, cameraTestable.getCameraAngles().pitch + offset, ctorCameraAngles.roll };
    cameraTestable.ProcessMouseMovement(offset, offset, false);
    CameraAngles actualCameraAngles = cameraTestable.getCameraAngles();
    EXPECT_EQ(expectedCameraAngles, cameraTestable.getCameraAngles());
}

// ------------------------------------------------------------------------
TEST_F(Cameratest, ProcessMouseScrolllCorrectlyUpdatesZoomValue)
{
    CameraAngles ctorCameraAngles = { 1.0f, 1.0f, 1.0f };
    CameraOptions ctorCameraOptions = { 1.0f, 1.0f, 1.0f };
    CameraTestable cameraTestable({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, ctorCameraAngles, ctorCameraOptions);

    // Valid scroll value
    float scrollValue = -5.0f;
    cameraTestable.ProcessMouseScroll(scrollValue);
    CameraOptions expectedCameraOptions = { ctorCameraOptions.mouseSensitivity, ctorCameraOptions.movementSpeed, ctorCameraOptions.zoom - scrollValue };
    EXPECT_EQ(expectedCameraOptions, cameraTestable.getCameraOptions());

    // High scroll value should be clipped to max scroll value
    scrollValue = cameraTestable.getMaxZoom() + 10.0f;
    expectedCameraOptions = { ctorCameraOptions.mouseSensitivity, ctorCameraOptions.movementSpeed, cameraTestable.getMinZoom() };
    cameraTestable.ProcessMouseScroll(scrollValue);
    EXPECT_EQ(expectedCameraOptions, cameraTestable.getCameraOptions());

    // Low scroll value should be clipped to min scroll value
    scrollValue = -1.0f * (cameraTestable.getMaxZoom() + 10.0f);
    expectedCameraOptions = { ctorCameraOptions.mouseSensitivity, ctorCameraOptions.movementSpeed, cameraTestable.getMaxZoom() };
    cameraTestable.ProcessMouseScroll(scrollValue);
    EXPECT_EQ(expectedCameraOptions, cameraTestable.getCameraOptions());
}

// ===============================================================
// FirstPersonCameraTestable
// ===============================================================
class FirstPersonCameraTestable : public FirstPersonCamera {
public:
    FirstPersonCameraTestable(MathUtils::Vec3 position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3 front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, CameraAngles cameraAngles = { 0.0f, -20.0f, 0.0f }, CameraOptions cameraOptions = { 2.5f, 0.5f, 45.0f })
        : FirstPersonCamera(position, front, cameraAngles, cameraOptions) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    CameraAttributes getCameraAttribues() { return m_cameraAttributes; }
    WorldAxis getWorldAxis() { return m_worldAxis; }
    CameraAngles getCameraAngles() { return m_cameraAngles; }
    CameraOptions getCameraOptions() { return m_cameraOptions; }
};

// ------------------------------------------------------------------------
TEST_F(Cameratest, FirstPersonCameraCorrectlyProcessesKeyboardInput)
{
    FirstPersonCameraTestable firstPersonCameraTestable;

    static constexpr float uniformMovementDelta = 0.4f;
    firstPersonCameraTestable.ProcessKeyboard(CameraMovement::FORWARD, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(0.0f, -0.342020154f, -0.939692736f), firstPersonCameraTestable.getCameraAttribues().position);

    firstPersonCameraTestable.ProcessKeyboard(CameraMovement::BACKWARD, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), firstPersonCameraTestable.getCameraAttribues().position);

    firstPersonCameraTestable.ProcessKeyboard(CameraMovement::RIGHT, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(1.0f, 0.0f, 0.0f), firstPersonCameraTestable.getCameraAttribues().position);

    firstPersonCameraTestable.ProcessKeyboard(CameraMovement::LEFT, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), firstPersonCameraTestable.getCameraAttribues().position);

    firstPersonCameraTestable.ProcessKeyboard(CameraMovement::ROLL_LEFT, uniformMovementDelta); // FUT
    EXPECT_EQ(25.0f, firstPersonCameraTestable.getCameraAngles().roll);

    firstPersonCameraTestable.ProcessKeyboard(CameraMovement::ROLL_RIGHT, uniformMovementDelta); // FUT
    EXPECT_EQ(0.0f, firstPersonCameraTestable.getCameraAngles().roll);
}

// ------------------------------------------------------------------------
TEST_F(Cameratest, FirstPersonCameraCorrectlyComputesViewMatrix)
{
    FirstPersonCameraTestable firstPersonCameraTestable;

    std::array<float, MathUtils::MAT4_SIZE> expectedViewMatrix =
    {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.939692616f, 0.342020124f, 0.0f,
            0.0f, -0.342020124f, 0.939692616f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
    };

    // FUT
    std::array<float, MathUtils::MAT4_SIZE> viewMatrix = firstPersonCameraTestable.FirstPersonCamera::GetViewMatrix();
    EXPECT_EQ(expectedViewMatrix, viewMatrix);
}

// ===============================================================
// ThirdPersonCameraTestable
// ===============================================================
class ThirdPersonCameraTestable : public ThirdPersonCamera {
public:
    ThirdPersonCameraTestable(MathUtils::Vec3 position = MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3 front = MathUtils::Vec3{ 0.0f, 0.0f, -1.0f }, CameraAngles cameraAngles = { 0.0f, -20.0f, 0.0f }, CameraOptions cameraOptions = { 2.5f, 0.5f, 45.0f })
        : ThirdPersonCamera(position, front, cameraAngles, cameraOptions) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    CameraAttributes getCameraAttribues() { return m_cameraAttributes; }
    WorldAxis getWorldAxis() { return m_worldAxis; }
    CameraAngles getCameraAngles() { return m_cameraAngles; }
    CameraOptions getCameraOptions() { return m_cameraOptions; }
};

// ------------------------------------------------------------------------
TEST_F(Cameratest, ThirdPersonCameraCorrectlyProcessesKeyboardInput)
{
    ThirdPersonCameraTestable thirdPersonCameraTestable;

    static constexpr float uniformMovementDelta = 0.4f;
    thirdPersonCameraTestable.ProcessKeyboard(CameraMovement::FORWARD, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(0.0f, 1.02606046f, 2.81907821f), thirdPersonCameraTestable.getCameraAttribues().position);
    EXPECT_EQ(glm::vec3(0.0f, -0.342020154f, -0.939692736f), thirdPersonCameraTestable.getCameraAttribues().target);

    thirdPersonCameraTestable.ProcessKeyboard(CameraMovement::BACKWARD, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(0.0f, 1.36808062f, 3.75877094f), thirdPersonCameraTestable.getCameraAttribues().position);
    EXPECT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), thirdPersonCameraTestable.getCameraAttribues().target);

    thirdPersonCameraTestable.ProcessKeyboard(CameraMovement::RIGHT, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(1.0f, 1.36808062f, 3.75877094f), thirdPersonCameraTestable.getCameraAttribues().position);
    EXPECT_EQ(glm::vec3(1.0f, 0.0f, 0.0f), thirdPersonCameraTestable.getCameraAttribues().target);

    thirdPersonCameraTestable.ProcessKeyboard(CameraMovement::LEFT, uniformMovementDelta); // FUT
    EXPECT_EQ(glm::vec3(0.0f, 1.36808062f, 3.75877094f), thirdPersonCameraTestable.getCameraAttribues().position);
    EXPECT_EQ(glm::vec3(0.0f, 0.0f, 0.0f), thirdPersonCameraTestable.getCameraAttribues().target);
}

// ------------------------------------------------------------------------
TEST_F(Cameratest, ThirdPersonCameraCorrectlyComputesViewMatrix)
{
    ThirdPersonCameraTestable thirdPersonCameraTestable;

    std::array<float, MathUtils::MAT4_SIZE> expectedViewMatrix =
    {
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 0.939692616f, 0.342020124f, 0.0f,
            0.0f, -0.342020124f, 0.939692616f, 0.0f,
            0.0f, 0.0f, -4.00000048f, 1.0f
    };

    // FUT
    std::array<float, MathUtils::MAT4_SIZE> viewMatrix = thirdPersonCameraTestable.GetViewMatrix();
    EXPECT_EQ(expectedViewMatrix, viewMatrix);
}
