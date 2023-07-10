#include "TextObject.hpp"

// ------------------------------------------------------------------------
TextObject::TextObject(const std::shared_ptr<Shader> shader, const std::shared_ptr<TextRasterizer> rasterizer, const TextProperties& textProperties)
    : RenderObject(shader)
    , m_rasterizer(rasterizer)
    , m_textProperties(textProperties)
{
    LOG(INFO) << "ctor";
    
    // Pre-render all possible common characters to render
    m_rasterizer->createNewVAO(shader);
}

// ------------------------------------------------------------------------
TextObject::~TextObject()
{
    LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void TextObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
    // Activate text shader and render text
    m_shader->use();
    m_shader->setVec3("textColor", &m_textProperties.color.x);

    m_rasterizer->drawArrays(m_textProperties);

    LOG(INFO) << "Render TextObject";
}

// ------------------------------------------------------------------------
void TextObject::updateTextProperties(const TextProperties& newTextProperties)
{
    m_textProperties = newTextProperties;
}
