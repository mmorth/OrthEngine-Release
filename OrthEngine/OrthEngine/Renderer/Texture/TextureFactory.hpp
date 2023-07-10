#ifndef TEXTUREFACTORY_HPP
#define TEXTUREFACTORY_HPP

#include <array>
#include <format>
#include <iostream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <glad/glad.h>
#include <g3log/g3log.hpp>
#include <g3log/loglevels.hpp>
#include <g3log/logworker.hpp>

#include "../Externals/stb_image/stb_image.h"

struct TextureData 
{
	unsigned int id;
	std::vector<unsigned char> data;
};

struct TextureArrayData
{
	std::string lookupName;
	std::vector<std::string> textureNames;
};

constexpr unsigned int NUM_SKYBOX_IMAGES = 6;
struct SkyboxData
{
	std::string skyboxName;
	std::array<std::string, NUM_SKYBOX_IMAGES> textureNames;
};

class TextureFactory 
{
public:
	TextureFactory();
	~TextureFactory();

	// constructor/destructor functions
	virtual void createDefaultTextures();
	virtual void cleanUp();

	// texture generator functions
	virtual unsigned int createNewTexture(const std::string& path);
	virtual unsigned int generateTextureArray(const TextureArrayData& textureArrayData);
	virtual unsigned int loadSkybox(const SkyboxData& skyboxData);

	// texture accessor functions
	virtual std::optional<unsigned int> getTextureID(const std::string& key);
	virtual unsigned int storeTextureData(const unsigned int textureID, const std::string& path, const std::vector<unsigned char>& textureByteData);

protected:
	// default texture data
	static std::vector<std::string> s_defaultTextures;
	static std::vector<SkyboxData> s_defaultSkybox;
	static std::vector<TextureArrayData> s_defaultTextureArray;

private:
	// texture data
	unsigned int m_numTextures;

	std::unordered_map<std::string, TextureData> m_textureData;
};

#endif // TEXTUREFACTORY_HPP