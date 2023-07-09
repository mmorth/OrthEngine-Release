#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "OpenGLFixture.hpp"

#include "Camera/Camera.hpp"
#include "CameraMock.hpp"

#include "FactoryMock.hpp"

#include "Publisher.hpp"
#include "PublisherMock.hpp"

#include "Managers/RenderObjectManager.hpp"
#include "Managers/tests/ManagerMock.hpp"

#include "Window/Window.hpp"
#include "Window/tests/WindowMock.hpp"

#include "EngineApp.hpp"

// ===============================================================
// EngineAppTestable
// ===============================================================
class EngineAppTestable : public EngineApp
{
public:
    EngineAppTestable()
        : EngineApp() {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<Window> getWindow() { return m_window; }
    std::shared_ptr<RenderObjectManager> getRenderObjectManager() { return m_renderObjectManager; }
    std::shared_ptr<Camera> getCamera() { return m_camera; }
    float getDeltaTime() { return m_deltaTime; }
    float getLastFrame() { return m_lastFrame; }

    void computeFps(std::chrono::time_point<std::chrono::high_resolution_clock>& start, unsigned int& frames) { return EngineApp::calculateFps(start, frames); }

    // ===============================================================
    // Mock Methods
    // ===============================================================
    MOCK_METHOD(std::chrono::time_point<std::chrono::high_resolution_clock>, getCurrentTime, ());
};

// ------------------------------------------------------------------------
class EngineAppTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<WindowMock> s_windowMock;
    static std::shared_ptr<RenderObjectManagerMock> s_renderObjectManagerMock;
    static std::shared_ptr<CameraMock> s_cameraMock;
    static ::testing::StrictMock<EngineAppTestable>* s_engineAppTestable;

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_windowMock = std::make_shared<WindowMock>();
        s_renderObjectManagerMock = std::make_shared<RenderObjectManagerMock>();
        s_cameraMock = std::make_shared<CameraMock>();

        s_engineAppTestable = new ::testing::StrictMock<EngineAppTestable>();
        s_engineAppTestable->initialize(s_windowMock, s_renderObjectManagerMock, s_cameraMock);
    }

    static void TearDownTestCase()
    {
        s_windowMock.reset();
        s_renderObjectManagerMock.reset();
        s_cameraMock.reset();
        delete s_engineAppTestable;

        OpenGLTestFixture::TearDownTestCase();
    }

};

std::shared_ptr<WindowMock> EngineAppTest::s_windowMock;
std::shared_ptr<RenderObjectManagerMock> EngineAppTest::s_renderObjectManagerMock;
std::shared_ptr<CameraMock> EngineAppTest::s_cameraMock;
::testing::StrictMock<EngineAppTestable>* EngineAppTest::s_engineAppTestable;

// ------------------------------------------------------------------------
TEST_F(EngineAppTest, CreateEngineAppCorrectlySetsDefaultValuesAndPublishers)
{
    // verify members
    EXPECT_EQ(s_windowMock, s_engineAppTestable->getWindow());
    EXPECT_EQ(s_renderObjectManagerMock, s_engineAppTestable->getRenderObjectManager());
    EXPECT_EQ(s_cameraMock, s_engineAppTestable->getCamera());
    EXPECT_EQ(0.0f, s_engineAppTestable->getDeltaTime());
    EXPECT_EQ(0.0f, s_engineAppTestable->getLastFrame());

    // verify cursor publisher
    float xPos = 1.0f; float yPos = 2.0f;
    EXPECT_CALL(*s_cameraMock, ProcessMouseMovement(::testing::FloatEq(xPos - GlobalSettings::getInstance().getValue<float>("windowWidth", 1200)/2), ::testing::FloatEq(GlobalSettings::getInstance().getValue<float>("windowHeight", 800)/2 - yPos), ::testing::Eq(true))).Times(1);
    s_windowMock->s_cursorPosPublisher->publish(std::pair<double, double>(xPos, yPos));

    // verify scroll publisher
    float yScroll = 1.0f;
    EXPECT_CALL(*s_cameraMock, ProcessMouseScroll(::testing::FloatEq(yScroll))).Times(1);
    s_windowMock->s_scrollPosPublisher->publish(std::pair<double, double>(0.0f, yScroll));

    // verify window size publisher
    int windowWidth = 800; int windowHeight = 600;
    EXPECT_CALL(*s_renderObjectManagerMock, updateFramebufferSize(::testing::Eq(windowWidth), ::testing::Eq(windowHeight))).Times(1);
    s_windowMock->s_windowSizePublisher->publish(std::pair<int, int>(windowWidth, windowHeight));
}

// ------------------------------------------------------------------------
TEST_F(EngineAppTest, RunEngineAppCallsCorrectMocks)
{
    // ake the main render loop only run once
    EXPECT_CALL(*s_windowMock, shouldCloseWindow())
        .WillOnce(::testing::Return(false))
        .WillOnce(::testing::Return(true));

    // Verify correct mock calls made
    EXPECT_CALL(*s_windowMock, getGlfwTime()).Times(1);
    EXPECT_CALL(*s_windowMock, processInput()).Times(1);
    EXPECT_CALL(*s_cameraMock, ProcessKeyboard(::testing::_, ::testing::_)).Times(1);
    EXPECT_CALL(*s_windowMock, getWindowSize(::testing::_, ::testing::_)).Times(1);
    EXPECT_CALL(*s_cameraMock, GetPosition()).Times(1);
    EXPECT_CALL(*s_cameraMock, GetFront()).Times(1);
    EXPECT_CALL(*s_renderObjectManagerMock, renderAll(::testing::_, ::testing::_)).Times(1);
    EXPECT_CALL(*s_renderObjectManagerMock, getTransformationMatrices(::testing::_, ::testing::_, ::testing::_)).Times(1);
    EXPECT_CALL(*s_windowMock, swapBuffers()).Times(1);
    EXPECT_CALL(*s_windowMock, pollEvents()).Times(1);
    EXPECT_CALL(*s_engineAppTestable, getCurrentTime()).Times(1)
        .WillOnce(::testing::Return(std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::seconds(1))));

    s_engineAppTestable->run();
}

// ------------------------------------------------------------------------
TEST_F(EngineAppTest, CalulateFpsCorrectlyComputesAndDrawsFps)
{
    // Set the initial time and frame count
    std::chrono::time_point<std::chrono::high_resolution_clock> start = std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::seconds(1));
    unsigned int frames = 0;

    EXPECT_CALL(*s_engineAppTestable, getCurrentTime())
        .WillOnce(::testing::Return(std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::seconds(1))))
        .WillOnce(::testing::Return(std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::seconds(1))));

    // Test case 1: elapsed time less than 1 second
    EXPECT_CALL(*s_renderObjectManagerMock, updateFpsText(testing::_)).Times(0);
    EXPECT_CALL(*s_windowMock, setWindowTitle(testing::_)).Times(0);
    s_engineAppTestable->computeFps(start, frames);

    start = std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::seconds(0));
    frames = 0;

    // Test case 2: elapsed time equal to 1 second
    EXPECT_CALL(*s_renderObjectManagerMock, updateFpsText(::testing::Eq("FPS: 1.000000"))).Times(1);
    EXPECT_CALL(*s_windowMock, setWindowTitle("FPS: 1.000000")).Times(1);
    s_engineAppTestable->computeFps(start, frames);
}
