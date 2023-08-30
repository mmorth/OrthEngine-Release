#ifndef NONINSTANCEDOBJECT_HPP
#define NONINSTANCEDOBJECT_HPP

#include <array>

#include "ObjectStructs.hpp"
#include "Rasterizers/NonInstancedRasterizer.hpp"
#include "RenderObject.hpp"
#include "Transform/Transform.hpp"

class NonInstancedObject : public RenderObject 
{
public:
    NonInstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<NonInstancedRasterizer> rasterizerPtr, const ObjectLocationOrientation& objectLocation);
    ~NonInstancedObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;
    virtual void drawNormals(const std::shared_ptr<Shader> shaderPtr, std::array<float, MathUtils::MAT4_SIZE> projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix) override;

    virtual void updateLocation(const MathUtils::Vec3& newPosition);
    virtual ObjectLocationOrientation getObjectLocationOrientation();

protected:
    void setTransform(const std::shared_ptr<Transform> transform);

    std::shared_ptr<NonInstancedRasterizer> m_rasterizer;

    ObjectLocationOrientation m_objectLocation;
    std::shared_ptr<Transform> m_transform;

    unsigned int m_VAO;

private:
    void renderObject(const std::shared_ptr<Shader> shaderPtr, const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix);

};

#endif // NONINSTANCEDOBJECT_HPP
