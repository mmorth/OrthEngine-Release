#ifndef GEOMETRICNONINSTANCEDOBJECT_HPP
#define GEOMETRICNONINSTANCEDOBJECT_HPP

#include "GeometricObjectStructs.hpp"
#include "RenderObjects/NonInstancedObject.hpp"

class GeometricNonInstancedObject : public NonInstancedObject 
{
public:
    GeometricNonInstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<NonInstancedRasterizer> rasterizer, const ObjectLocationOrientation& objectLocation, const ObjectMaterialProperties& objectMaterialProperties);
    ~GeometricNonInstancedObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;

protected:
    ObjectMaterialProperties m_objectMaterialProperties;

};

#endif // GEOMETRICNONINSTANCEDOBJECT_HPP
