#ifndef TEXTUREFACTORYMOCK_HPP
#define TEXTUREFACTORYMOCK_HPP

#include <gmock/gmock.h>

#include "Texture/TextureFactory.hpp"

class TextureFactoryMock : public TextureFactory
{
public:
    MOCK_METHOD(void, createDefaultTextures, (), (override));
    MOCK_METHOD(void, cleanUp, (), (override));
    MOCK_METHOD(unsigned int, createNewTexture, (const std::string&), (override));
    MOCK_METHOD(unsigned int, generateTextureArray, (const TextureArrayData&), (override));
    MOCK_METHOD(unsigned int, loadSkybox, (const SkyboxData&), (override));
    MOCK_METHOD(std::optional<unsigned int>, getTextureID, (const std::string&), (override));
    MOCK_METHOD(unsigned int, storeTextureData, (const unsigned int, const std::string&, const std::vector<unsigned char>&), (override));
};

#endif // TEXTUREFACTORYMOCK_HPP
