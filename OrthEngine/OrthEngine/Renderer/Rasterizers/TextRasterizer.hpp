#ifndef TEXTURERASTERIZER_HPP
#define TEXTURERASTERIZER_HPP

#include <map>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "GlobalSettingsConstants/GlobalSettings.hpp"
#include "MathUtils.hpp"
#include "Shader/Shader.hpp"
#include "Rasterizers/Rasterizer.hpp"
#include "RenderObjects/OtherRenders/OtherRenderStructs.hpp"

struct Character 
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2   Size;      // Size of glyph
    glm::ivec2   Bearing;   // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

class TextRasterizer : public Rasterizer
{
public:
    TextRasterizer(const VertexData& vertexProperties);
    ~TextRasterizer();

    virtual unsigned int createNewVAO(const std::shared_ptr<Shader>& shader);
    virtual void drawArrays(const TextProperties& textProperties);

protected:
    virtual float renderCharacter(const char c, const TextProperties& textProperties, const float originalXPos);

    unsigned int m_VAO;
    unsigned int m_VBO;

    std::map<GLchar, Character> m_characters;

};

#endif // TEXTURERASTERIZER_HPP
