#include "gtest/gtest.h"

#include "GlobalSettings.hpp"

class GlobalSettingsTest : public testing::Test
{
protected:
    virtual void SetUp() {}

    virtual void TearDown() {}
};

TEST(GlobalSettingsTest, GlobalSettingsCorrectlyReadFromJsonFile)
{
    EXPECT_EQ(true, GlobalSettings::getInstance().getValue<bool>("drawNormals", false));
    EXPECT_EQ(false, GlobalSettings::getInstance().getValue<bool>("framebufferEnabled", true));
    EXPECT_EQ(false, GlobalSettings::getInstance().getValue<bool>("msaaEnabled", true));
    EXPECT_EQ(true, GlobalSettings::getInstance().getValue<bool>("displayFps", false));
    EXPECT_EQ(true, GlobalSettings::getInstance().getValue<bool>("depthState", false));
    EXPECT_EQ(true, GlobalSettings::getInstance().getValue<bool>("blendState", false));
    EXPECT_EQ(false, GlobalSettings::getInstance().getValue<bool>("wireframeState", true));
    EXPECT_EQ(1200, GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 0));
    EXPECT_EQ(800, GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 0));
    EXPECT_EQ("OrthEngine", GlobalSettings::getInstance().getValue<std::string>("windowName", ""));
    EXPECT_EQ(true, GlobalSettings::getInstance().getValue<bool>("limitFps", false));
    EXPECT_EQ(3, GlobalSettings::getInstance().getValue<unsigned int>("majorVersion", 0));
    EXPECT_EQ(3, GlobalSettings::getInstance().getValue<unsigned int>("minorVersion", 0));
    EXPECT_EQ(4, GlobalSettings::getInstance().getValue<unsigned int>("numSamples", 0));
    EXPECT_EQ(false, GlobalSettings::getInstance().getValue<bool>("testingError", true));
}
