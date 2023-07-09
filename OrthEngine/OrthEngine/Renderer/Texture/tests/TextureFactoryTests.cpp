#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "OpenGLFixture.hpp"
#include "Texture/TextureFactory.hpp"

// ===============================================================
// TextureFactoryTestable
// ===============================================================
class TextureFactoryTestable : public TextureFactory {
public:
    TextureFactoryTestable() : TextureFactory() {}

    // ===============================================================
    // Texture Data Accessors
    // ===============================================================
    static std::vector<std::string> getDefaultTextures()
    {
        return TextureFactory::s_defaultTextures;
    }

    static std::vector<SkyboxData> getDefaultSkybox()
    {
        return TextureFactory::s_defaultSkybox;
    }

    static std::vector<TextureArrayData> getDefaultTextureArray()
    {
        return TextureFactory::s_defaultTextureArray;
    }
};

// ------------------------------------------------------------------------
class TextureFactoryTest : public OpenGLTestFixture
{
public:
    static ::testing::StrictMock<TextureFactoryTestable>* s_textureFactoryTestable;

protected:
    void SetUp() override {}
    void TearDown() override {}

    static void SetUpTestCase() 
    {
        OpenGLTestFixture::SetUpTestCase();
        s_textureFactoryTestable = new ::testing::StrictMock<TextureFactoryTestable>();
    }

    static void TearDownTestCase() 
    {
        delete s_textureFactoryTestable;
        OpenGLTestFixture::TearDownTestCase();
    }

    void verifyTextureValid(::testing::StrictMock<TextureFactoryTestable>& textureFactoryTestable, std::string textureName)
    {
        std::optional<unsigned int> textureIdOpt = s_textureFactoryTestable->getTextureID(textureName);
        EXPECT_EQ(true, textureIdOpt.has_value());
        unsigned int textureId = textureIdOpt.value();
        EXPECT_TRUE(textureId > 0);
        EXPECT_EQ(GL_TRUE, glIsTexture(textureId));
    }
};

::testing::StrictMock<TextureFactoryTestable>* TextureFactoryTest::s_textureFactoryTestable = nullptr;

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCorrectlyCreatesDefaultTextures)
{
    // Test that default textures are created in constructor
    EXPECT_TRUE(TextureFactoryTestable::getDefaultTextures().size() > 0);
    for (auto defaultTexture : TextureFactoryTestable::getDefaultTextures())
    {
        verifyTextureValid(*s_textureFactoryTestable, defaultTexture);
    }
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCorrectlyCreatesDefaultSkybox)
{
    // Test that default textures are created in constructor
    EXPECT_TRUE(TextureFactoryTestable::getDefaultSkybox().size() > 0);
    for (auto defaultTexture : TextureFactoryTestable::getDefaultSkybox())
    {
        verifyTextureValid(*s_textureFactoryTestable, defaultTexture.skyboxName);
    }
}


// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCorrectlyCreatesDefaultTextureArrays)
{
    // Test that default textures are created in constructor
    EXPECT_TRUE(TextureFactoryTestable::getDefaultTextureArray().size() > 0);
    for (auto defaultTexture : TextureFactoryTestable::getDefaultTextureArray())
    {
        verifyTextureValid(*s_textureFactoryTestable, defaultTexture.lookupName);
    }
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateTextureProperlyCreatesNewTexture)
{
    std::string textureName = "test.PNG";
    unsigned int textureID = s_textureFactoryTestable->createNewTexture(textureName); // FUT
    EXPECT_TRUE(textureID > 0);
    verifyTextureValid(*s_textureFactoryTestable, textureName);
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateTextureArrayProperlyCreatesNewTexture)
{
    // create dummy texture for texture array
    std::string textureName = "test.PNG";
    unsigned int textureID = s_textureFactoryTestable->createNewTexture(textureName);

    // Create and verify texture array
    std::string textureArrayName = "test_array";
    TextureArrayData textureArrayData = { textureArrayName, {textureName}};
    s_textureFactoryTestable->generateTextureArray(textureArrayData); // FUT
    verifyTextureValid(*s_textureFactoryTestable, textureArrayName);
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateSkyboxProperlyCreatesNewTexture)
{
    SkyboxData skyboxData = { "Test_Skybox", { "test.PNG", "test.PNG", "test.PNG", "test.PNG", "test.PNG", "test.PNG" } };
    unsigned int skyboxID = s_textureFactoryTestable->loadSkybox(skyboxData);
    verifyTextureValid(*s_textureFactoryTestable, skyboxData.skyboxName);
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateTextureWithInvalidDataThrowsException)
{
    std::string textureName = "test_fake_img.PNG";
    EXPECT_THROW(s_textureFactoryTestable->createNewTexture(textureName), std::runtime_error); // FUT
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateTextureArrayWithNonMatchingTextureDimensionsThrowsException)
{
    std::string textureName = "test.PNG";
    std::string textureNameResized = "test_resized.PNG";
    s_textureFactoryTestable->createNewTexture(textureName);
    s_textureFactoryTestable->createNewTexture(textureNameResized);

    // Create and verify texture array
    std::string textureArrayName = "test_array_unmatching_dimensions";
    TextureArrayData textureArrayData = { textureArrayName, { textureName, textureNameResized }};
    EXPECT_THROW(s_textureFactoryTestable->generateTextureArray(textureArrayData), std::runtime_error); // FUT
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateTextureArrayWithNonMatchingImageFormatThrowsException)
{
    std::string textureName = "test.PNG";
    std::string textureNameJPG = "test_jpg.jpg";
    s_textureFactoryTestable->createNewTexture(textureName);
    s_textureFactoryTestable->createNewTexture(textureNameJPG);

    // Create and verify texture array
    std::string textureArrayName = "test_array_unmatching_dimensions";
    TextureArrayData textureArrayData = { textureArrayName, { textureName, textureNameJPG } };
    EXPECT_THROW(s_textureFactoryTestable->generateTextureArray(textureArrayData), std::runtime_error); // FUT
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateSkyboxWithNonMatchingDimensionsThrowsExpection)
{
    SkyboxData skyboxData = { "Test_Skybox", { "test.PNG", "test_resized.PNG", "test.PNG", "test.PNG", "test.PNG", "test.PNG" } };
    EXPECT_THROW(s_textureFactoryTestable->loadSkybox(skyboxData), std::runtime_error); // FUT
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateSkyboxWithNonMatchingImageFormatThrowsExpection)
{
    SkyboxData skyboxData = { "Test_Skybox", { "test.PNG", "test_jpg.jpg", "test.PNG", "test.PNG", "test.PNG", "test.PNG" } };
    EXPECT_THROW(s_textureFactoryTestable->loadSkybox(skyboxData), std::runtime_error); // FUT
}

// ------------------------------------------------------------------------
TEST_F(TextureFactoryTest, TextureFactoryCreateSkyboxWithInvalidDataThrowsExpection)
{
    SkyboxData skyboxData = { "Test_Skybox", { "test.PNG", "test_invalid_data.jpg", "test.PNG", "test.PNG", "test.PNG", "test.PNG" } };
    EXPECT_THROW(s_textureFactoryTestable->loadSkybox(skyboxData), std::runtime_error); // FUT
}
