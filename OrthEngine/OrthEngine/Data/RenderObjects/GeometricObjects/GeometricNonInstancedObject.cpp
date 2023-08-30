#include "GeometricNonInstancedObject.hpp"

// ------------------------------------------------------------------------
GeometricNonInstancedObject::GeometricNonInstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<NonInstancedRasterizer> rasterizer, const ObjectLocationOrientation& objectLocation, const ObjectMaterialProperties& objectMaterialProperties)
	: NonInstancedObject(shaderPtr, rasterizer, objectLocation)
	, m_objectMaterialProperties(objectMaterialProperties)
{
	LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
GeometricNonInstancedObject::~GeometricNonInstancedObject()
{
	LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void GeometricNonInstancedObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
	// Run texture and set material properties in the shader
	TextureMaps textureMaps = { m_objectMaterialProperties.diffuseMap, m_objectMaterialProperties.specularMap };
	m_rasterizer->activateTextures(textureMaps);
	m_shader->use();
	m_shader->setInt("material.diffuseMap", m_objectMaterialProperties.diffuseMap);
	m_shader->setInt("material.specularMap", m_objectMaterialProperties.specularMap);
	m_shader->setFloat("material.shininess", m_objectMaterialProperties.shininess);

	NonInstancedObject::render(projectionMatrix, viewMatrix);

	LOG(INFO) << "Render GeometricNonInstancedObject";
}