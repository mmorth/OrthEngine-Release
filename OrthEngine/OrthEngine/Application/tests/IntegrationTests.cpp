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
    EngineAppTestRenderTestable(bool isTestRender)
        : EngineApp(isTestRender) {}

    // ===============================================================
    // Protected Accessors
    // ===============================================================
    std::shared_ptr<RenderObjectManager> getRenderObjectManager() { return m_renderObjectManager; }
};

// ------------------------------------------------------------------------
class IntegrationTest : public OpenGLTestFixture
{
public:
    static ::testing::StrictMock<EngineAppTestRenderTestable>* s_engineAppTestable;

    // ------------------------------------------------------------------------
    void writeVectorToFile(const std::vector<unsigned char>& data, const std::string& filename) 
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
        file.write(reinterpret_cast<const char*>(data.data()), sizeof(unsigned char) * size);

        file.close();
    }

    // ------------------------------------------------------------------------
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
        file.read(reinterpret_cast<char*>(data.data()), sizeof(unsigned char) * size);

        file.close();

        return data;
    }

    // ------------------------------------------------------------------------
    double renderComparison(std::vector<unsigned char> referencePixels, std::vector<unsigned char> testPixels)
    {
        unsigned int squaredDifference = 0.0;
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

        s_engineAppTestable = new ::testing::StrictMock<EngineAppTestRenderTestable>(true);
    }

    static void TearDownTestCase()
    {
        delete s_engineAppTestable;

        OpenGLTestFixture::TearDownTestCase();
    }

};

::testing::StrictMock<EngineAppTestRenderTestable>* IntegrationTest::s_engineAppTestable;

// ------------------------------------------------------------------------
TEST_F(IntegrationTest, RunEngineAppCallsCorrectMocks)
{
    s_engineAppTestable->run();

    std::vector<unsigned char> testPixels = s_engineAppTestable->getRenderObjectManager()->getFramebufferContent();

    // TODO: Make this more user-friendly to avoid having to uncomment this every time CMake is reran
    //writeVectorToFile(testPixels, "ReferenceRenderBuffer.data");

    std::vector<unsigned char> referencePixels = readVectorFromFile("ReferenceRenderBuffer.data");

    unsigned int diffThreshold = 10;
    unsigned int testRefDiff = renderComparison(referencePixels, testPixels);
    std::cout << "testRefDiff = " << testRefDiff << "\n";
    EXPECT_TRUE(testRefDiff < diffThreshold);
}
