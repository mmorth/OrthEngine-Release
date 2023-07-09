#include "GeometricInstancedObject.hpp"

// ------------------------------------------------------------------------
GeometricInstancedObject::GeometricInstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer, const ObjectMaterialProperties& objectMaterialProperties)
	: InstancedObject(shaderPtr, rasterizer)
	, m_objectMaterialProperties(objectMaterialProperties)
{
	//LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
GeometricInstancedObject::~GeometricInstancedObject()
{
	//LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void GeometricInstancedObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
	// Run texture and set material properties in the shader
	TextureMaps textureMaps = { m_objectMaterialProperties.diffuseMap, m_objectMaterialProperties.specularMap };
	m_rasterizer->activateTextures(textureMaps);
	m_shader->use();
	m_shader->setInt("material.diffuseMapArray", m_objectMaterialProperties.diffuseMap);
	m_shader->setInt("material.specularMap", m_objectMaterialProperties.specularMap);
	m_shader->setFloat("material.shininess", m_objectMaterialProperties.shininess);

	InstancedObject::render(projectionMatrix, viewMatrix);

	//LOG(INFO) << "GeometricInstancedObject render";
}