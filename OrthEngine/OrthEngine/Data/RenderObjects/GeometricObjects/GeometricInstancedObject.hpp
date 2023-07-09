#ifndef GEOMETRICINSTANCEDOBJECT_HPP
#define GEOMETRICINSTANCEDOBJECT_HPP

#include "GeometricObjectStructs.hpp"
#include "RenderObjects/InstancedObject.hpp"

class GeometricInstancedObject : public InstancedObject 
{
public:
    GeometricInstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer, const ObjectMaterialProperties& objectMaterialProperties);
    ~GeometricInstancedObject();

    virtual void render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix) override;

protected:
    ObjectMaterialProperties m_objectMaterialProperties;

};

#endif // GEOMETRICINSTANCEDOBJECT_HPP
