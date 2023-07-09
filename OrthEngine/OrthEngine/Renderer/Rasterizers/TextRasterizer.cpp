#include "TextRasterizer.hpp"

// ------------------------------------------------------------------------
TextRasterizer::TextRasterizer(const VertexData& vertexProperties)
	: Rasterizer(vertexProperties)
    , m_VAO(0)
    , m_VBO(0)
    , m_characters()
{
    //LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
TextRasterizer::~TextRasterizer()
{
    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
unsigned int TextRasterizer::createNewVAO(const std::shared_ptr<Shader>& shader)
{
    // set ortho projection to have text lay flat on screen
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(GlobalSettings::getInstance().getValue<unsigned int>("windowWidth", 1200)), 0.0f, static_cast<float>(GlobalSettings::getInstance().getValue<unsigned int>("windowHeight", 800)));
    shader->use();
    shader->setMat4("projection", glm::value_ptr(projection));

    // initialze FreeType text rendering
    FT_Library ft;
    if (FT_Init_FreeType(&ft))
        throw std::runtime_error("Could not initialize FreeType library");

    // find path to font
    std::string font_name = "Antonio-Bold.ttf"; // TODO: Eventually make configurable
    if (font_name.empty())
        throw std::runtime_error("Failed to load font name: " + font_name);

    // load font as face
    FT_Face face;
    if (FT_New_Face(ft, font_name.c_str(), 0, &face))
        throw std::runtime_error("Failed to load font face");

    // set size font size
    FT_Set_Pixel_Sizes(face, 0, 48); // TODO: Eventually make configurable

    // disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

    // load first 128 characters of ASCII set for letters, numbers, and common puncutation into a map
    for (unsigned char c = 0; c < 128; c++)
    {
        // load glyph character
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
            throw std::runtime_error("Failed to load Glyph");

        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RED,
            face->glyph->bitmap.width,
            face->glyph->bitmap.rows,
            0,
            GL_RED,
            GL_UNSIGNED_BYTE,
            face->glyph->bitmap.buffer
        );

        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // save character to map
        Character character = {
            texture,
            glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
            glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
            static_cast<unsigned int>(face->glyph->advance.x)
        };
        m_characters.insert(std::pair<char, Character>(c, character));
    }
    glBindTexture(GL_TEXTURE_2D, 0);

    // free FreeType since we're done using it
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // create VAO and VBO for text quads
    glGenVertexArrays(1, &m_VAO);
    glGenBuffers(1, &m_VBO);
    glBindVertexArray(m_VAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    glBufferData(GL_ARRAY_BUFFER, 6 * 4 * sizeof(float), NULL, GL_DYNAMIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_VAOs.push_back(m_VAO);

    //LOG(INFO) << "New Text VAO ID " << m_VAO << " created";

    return m_VAO;
}

// ------------------------------------------------------------------------
void TextRasterizer::drawArrays(const TextProperties& textProperties)
{
    // Bind texture and VAO
    glActiveTexture(GL_TEXTURE0);
    glBindVertexArray(m_VAO);

    // iterate through all characters of text to render
    float originalXPos = textProperties.position.x;
    for (int i = 0; i < textProperties.text.size(); i++)
    {
        originalXPos = renderCharacter(textProperties.text[i], textProperties, originalXPos);
    }

    // Bind texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);

    //LOG(INFO) << "Draw TextRasterizer Arrays";
}

// ------------------------------------------------------------------------
float TextRasterizer::renderCharacter(const char c, const TextProperties& textProperties, const float originalXPos)
{
    // find character and positions to render
    Character ch = m_characters[c];
    float xpos = originalXPos + ch.Bearing.x * textProperties.scale;
    float ypos = textProperties.position.y - (ch.Size.y - ch.Bearing.y) * textProperties.scale;
    float w = ch.Size.x * textProperties.scale;
    float h = ch.Size.y * textProperties.scale;

    // update VBO for each character
    float vertices[6][4] = {
        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos,     ypos,       0.0f, 1.0f },
        { xpos + w, ypos,       1.0f, 1.0f },

        { xpos,     ypos + h,   0.0f, 0.0f },
        { xpos + w, ypos,       1.0f, 1.0f },
        { xpos + w, ypos + h,   1.0f, 0.0f }
    };

    // render glyph texture over quad
    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices); // be sure to use glBufferSubData and not glBufferData
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    return originalXPos + (ch.Advance >> 6) * textProperties.scale;
}
