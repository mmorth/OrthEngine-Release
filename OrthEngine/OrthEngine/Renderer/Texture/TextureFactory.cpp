#include "TextureFactory.hpp"

// Initialize static variables
std::vector<std::string> TextureFactory::s_defaultTextures = 
    { "container.jpg", "container2_resized.PNG", "awesomeface.png", "grass.png", "blending_transparent_window_resized.png", "container2_specular.PNG" };

std::vector<SkyboxData> TextureFactory::s_defaultSkybox = 
{
    { "Default_Skybox", { "skyboxRight.jpg", "skyboxLeft.jpg", "skyboxTop.jpg", "skyboxBottom.jpg", "skyboxFront.jpg", "skyboxBack.jpg" }}
};

std::vector<TextureArrayData> TextureFactory::s_defaultTextureArray = 
    { 
        {"Instanced_Texture_Array", { "container2_resized.PNG", "blending_transparent_window_resized.png" }} 
    };

// ------------------------------------------------------------------------
TextureFactory::TextureFactory()
    : m_numTextures(0)
{
    //LOG(INFO) << "ctor";

    createDefaultTextures();
}

// ------------------------------------------------------------------------
TextureFactory::~TextureFactory()
{
    cleanUp();

    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void TextureFactory::createDefaultTextures()
{
    // Create all the default textures
    for (const auto& textureName : s_defaultTextures)
        createNewTexture(textureName);

    // Skybox textures
    for (const auto& skyboxFaces : s_defaultSkybox)
        loadSkybox(skyboxFaces);

    // Textures arrays
    for (const auto& textureArrayData : s_defaultTextureArray)
        generateTextureArray(textureArrayData);

    //LOG(INFO) << "Default textures created";
}

// ------------------------------------------------------------------------
void TextureFactory::cleanUp()
{
    for (unsigned int textureID = 0; textureID < m_numTextures; textureID++)
        glDeleteTextures(1, &textureID);

    //LOG(INFO) << "Textures deleted";
}

// ------------------------------------------------------------------------
unsigned int TextureFactory::createNewTexture(const std::string& path)
{
    // Generate and load texture
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

    // Check for invalid data
    if (!data)
    {
        stbi_image_free(data);
        throw std::runtime_error("Texture failed to load at path: " + path);
    }

    // Set texture properties
    GLenum format = (nrComponents == 1) ? GL_RED : ((nrComponents == 3) ? GL_RGB : GL_RGBA);

    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    // Store texture and cleanup
    std::vector<unsigned char> textureVals(data, data + static_cast<std::size_t>(width) * height * nrComponents);

    stbi_image_free(data);

    //LOG(INFO) << "New texture ID " << textureID << "created";

    return storeTextureData(textureID, path, textureVals);
}

// ------------------------------------------------------------------------
unsigned int TextureFactory::generateTextureArray(const TextureArrayData& textureArrayData)
{
    // Generate and bind texture array
    GLuint textureArray;
    glGenTextures(1, &textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    // Upload texture data for each layer
    int width, height, format, numLayers;
    bool dimensionsInitialized = false;
    for (std::size_t layer = 0; layer < textureArrayData.textureNames.size(); layer++)
    {
        // Bind the texture and compute dimensions
        int curWidth, curHeight, curFormat;
        glBindTexture(GL_TEXTURE_2D, m_textureData[textureArrayData.textureNames[layer]].id);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &curWidth);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &curHeight);
        glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_INTERNAL_FORMAT, &curFormat);


        if (!dimensionsInitialized)
        {
            // Store the dimensions of the first texture for commparison
            width = curWidth; height = curHeight; format = curFormat;

            // Allocate storage for the texture array
            numLayers = static_cast<int>(textureArrayData.textureNames.size());
            glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, width, height, numLayers, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
            dimensionsInitialized = true;
        }

        // Check for valid texture and store to texture map if so
        if (curWidth != width || curHeight != height || curFormat != format)
            throw std::runtime_error("Dimensions of this texture do not match expected for texture array: " + textureArrayData.lookupName);

        const std::vector<unsigned char>& textureData = m_textureData[textureArrayData.textureNames[layer]].data;
        glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, layer, curWidth, curHeight, 1, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
    }

    // Set texture parameters if needed
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Bind the texture array to a texture unit
    glActiveTexture(GL_TEXTURE0 + textureArray);
    glBindTexture(GL_TEXTURE_2D_ARRAY, textureArray);

    //LOG(INFO) << "New texture array ID " << textureArray << " created";

    // Store texture array data and return texture array ID
    return storeTextureData(textureArray, textureArrayData.lookupName, std::vector<unsigned char>{});
}

// ------------------------------------------------------------------------
unsigned int TextureFactory::loadSkybox(const SkyboxData& skyboxData)
{
    // create the skybox texture
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    // load each face of the skybox
    int width, height, format;
    bool dimensionsInitialized = false;
    for (unsigned int i = 0; i < skyboxData.textureNames.size(); i++)
    {
        // Load and store skybox data
        int curWidth, curHeight, curFormat;
        unsigned char* data = stbi_load(skyboxData.textureNames[i].c_str(), &curWidth, &curHeight, &curFormat, 0);

        // Verify that all skybox faces have the same dimensions
        if (!dimensionsInitialized)
        {
            width = curWidth; height = curHeight; format = curFormat;
            dimensionsInitialized = true;
        }
        
        // Check for any errors in skybox initialization
        if (curWidth != width || curHeight != height || curFormat != format || !data)
        {
            stbi_image_free(data);
            throw std::runtime_error("Dimensions of this texture do not match expected for given skybox OR Cubemap texture failed to load at path: " + skyboxData.textureNames[i]);
        }

        // Create skybox image position
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, curWidth, curHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    }

    // set skybox to linear filter and slamping to edge to prevent sudden jumps
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    //LOG(INFO) << "New skybox ID " << textureID << " created";

    // Store the data
    return storeTextureData(textureID, skyboxData.skyboxName, std::vector<unsigned char>{});
}

// ------------------------------------------------------------------------
std::optional<unsigned int> TextureFactory::getTextureID(const std::string& key)
{
    auto it = m_textureData.find(key);
    if (it != m_textureData.end())
        return it->second.id;
    else
        return std::nullopt;
}

// ------------------------------------------------------------------------
unsigned int TextureFactory::storeTextureData(const unsigned int textureID, const std::string& path, const std::vector<unsigned char>& textureByteData)
{
    TextureData textureData = { textureID, textureByteData };
    m_textureData.emplace(path, std::move(textureData));

    m_numTextures = textureID;
    return textureID;
}
