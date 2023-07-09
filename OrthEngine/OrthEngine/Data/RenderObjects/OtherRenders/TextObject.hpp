#ifndef TEXTOBJECT_HPP
#define TEXTOBJECT_HPP

#include <map>
#include <memory>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include "MathUtils.hpp"
#include "Rasterizers/TextRasterizer.hpp"
#include "RenderObjects/OtherRenders/OtherRenderStructs.hpp"
#include "RenderObjects/RenderObject.hpp"
#include "Shader/Shader.hpp"

class TextObject : public RenderObject 
{
public:
    TextObject(const std::shared_ptr<Shader> shader, const std::shared_ptr<TextRasterizer> rasterizer, const TextProperties& textProperties);
    ~TextObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;

    virtual void updateTextProperties(const TextProperties& newTextProperties);

protected:
    std::shared_ptr<TextRasterizer> m_rasterizer;

    TextProperties m_textProperties;
};

#endif // TEXTOBJECT_HPP
