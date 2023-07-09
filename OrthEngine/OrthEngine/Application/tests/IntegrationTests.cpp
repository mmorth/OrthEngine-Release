#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "OpenGLFixture.hpp"

#include "EngineApp.hpp"

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "OpenGLFixture.hpp"

#include "Camera/Camera.hpp"
#include "CameraMock.hpp"

#include "FactoryMock.hpp"

#include "Publisher.hpp"
#include "PublisherMock.hpp"

#include "Window/Window.hpp"
#include "Window/tests/WindowMock.hpp"

#include "EngineApp.hpp"

constexpr int FRAMEBUFFER_WIDTH = 1200;
constexpr int FRAMEBUFFER_HEIGHT = 800;

// ===============================================================
// EngineAppTestRenderTestable
// ===============================================================
class EngineAppTestRenderTestable : public EngineApp
{
public:
    EngineAppTestRenderTestable()
        : EngineApp() {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<RenderObjectManager> getRenderObjectManager() { return m_renderObjectManager; }

    // ===============================================================
    // Mock Methods
    // ===============================================================
    MOCK_METHOD(std::chrono::time_point<std::chrono::high_resolution_clock>, getCurrentTime, ());
};

// ------------------------------------------------------------------------
class IntegrationTest : public OpenGLTestFixture
{
public:
    static std::shared_ptr<Window> s_window;
    static std::shared_ptr<RenderObjectManager> s_renderObjectManager;
    static std::shared_ptr<Camera> s_camera;
    static ::testing::StrictMock<EngineAppTestRenderTestable>* s_engineAppTestable;

    void writeVectorToFile(const std::vector<int>& data, const std::string& filename) 
    {
        std::ofstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        // Write the size of the vector
        size_t size = data.size();
        file.write(reinterpret_cast<const char*>(&size), sizeof(size_t));

        // Write the vector data
        file.write(reinterpret_cast<const char*>(data.data()), sizeof(int) * size);

        file.close();
    }

    std::vector<unsigned char> readVectorFromFile(const std::string& filename) 
    {
        std::ifstream file(filename, std::ios::binary);
        if (!file) {
            std::cerr << "Failed to open file for reading: " << filename << std::endl;
            return {};
        }

        // Read the size of the vector
        size_t size;
        file.read(reinterpret_cast<char*>(&size), sizeof(size_t));

        // Read the vector data
        std::vector<unsigned char> data(size);
        file.read(reinterpret_cast<char*>(data.data()), sizeof(int) * size);

        file.close();

        return data;
    }

    double renderComparison(std::vector<unsigned char> referencePixels, std::vector<unsigned char> testPixels)
    {
        double squaredDifference = 0.0;
        for (size_t i = 0; i < testPixels.size(); i += 4)
        {
            int redDiff = referencePixels[i] - testPixels[i];
            int greenDiff = referencePixels[i + 1] - testPixels[i + 1];
            int blueDiff = referencePixels[i + 2] - testPixels[i + 2];
            squaredDifference += redDiff * redDiff + greenDiff * greenDiff + blueDiff * blueDiff;
        }

        // Normalize the squared difference
        return squaredDifference / (FRAMEBUFFER_WIDTH * FRAMEBUFFER_HEIGHT);
    }

protected:
    static void SetUpTestCase()
    {
        OpenGLTestFixture::SetUpTestCase();
        s_window = std::make_shared<Window>();
        s_renderObjectManager = std::make_shared<RenderObjectManager>();
        s_camera = std::make_shared<Camera>();

        s_engineAppTestable = new ::testing::StrictMock<EngineAppTestRenderTestable>(true);
        s_engineAppTestable->initialize(s_window, s_renderObjectManager, s_camera);
    }

    static void TearDownTestCase()
    {
        s_window.reset();
        s_renderObjectManager.reset();
        s_camera.reset();
        delete s_engineAppTestable;

        OpenGLTestFixture::TearDownTestCase();
    }

};

std::shared_ptr<Window> IntegrationTest::s_window;
std::shared_ptr<RenderObjectManager> IntegrationTest::s_renderObjectManager;
std::shared_ptr<Camera> IntegrationTest::s_camera;
::testing::StrictMock<EngineAppTestRenderTestable>* IntegrationTest::s_engineAppTestable;

// ------------------------------------------------------------------------
TEST_F(IntegrationTest, RunEngineAppCallsCorrectMocks)
{
    EXPECT_CALL(*s_engineAppTestable, getCurrentTime()).Times(1)
        .WillOnce(::testing::Return(std::chrono::time_point<std::chrono::high_resolution_clock>(std::chrono::seconds(1))));

    s_engineAppTestable->run();

    std::vector<unsigned char> testPixels = s_engineAppTestable->getRenderObjectManager()->getFramebufferContent();
    std::vector<unsigned char> referencePixels = readVectorFromFile("ReferenceRenderBuffer.data");

    unsigned int diffThreshold = 100;
    unsigned int testRefDiff = renderComparison(referencePixels, testPixels);
    std::cout << "testRefDiff = " << testRefDiff << "\n";
    EXPECT_TRUE(testRefDiff < diffThreshold);
}
