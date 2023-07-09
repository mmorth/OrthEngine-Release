#include "InstancedObject.hpp"

// ------------------------------------------------------------------------
InstancedObject::InstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizerPtr)
	: RenderObject(shaderPtr)
	, m_rasterizer(rasterizerPtr)
	, m_modelMatrices()
	, m_textureIDs()
	, m_VAO(m_rasterizer->createNewVAO())
{
	//LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
InstancedObject::~InstancedObject()
{
	//LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void InstancedObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
	// Draw object with default object shader
	RenderObject::render(projectionMatrix, viewMatrix);
	renderObject(m_shader, projectionMatrix, viewMatrix);

	//LOG(INFO) << "Render InstancedObject";
}

// ------------------------------------------------------------------------
void InstancedObject::drawNormals(const std::shared_ptr<Shader> shaderPtr, std::array<float, MathUtils::MAT4_SIZE> projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix)
{
	// Draw object with normal shader
	RenderObject::drawNormals(shaderPtr, projectionMatrix, viewMatrix);
	renderObject(shaderPtr, projectionMatrix, viewMatrix);

	//LOG(INFO) << "Render NonInstancedObject Normals";
}

// ------------------------------------------------------------------------
void InstancedObject::addInstancedObject(const std::array<float, MathUtils::MAT4_SIZE>& modelMatrix, const float textureID)
{
	m_modelMatrices.push_back(modelMatrix);
	m_textureIDs.push_back(textureID);

	// Update instance buffers
	m_rasterizer->updateModelMatrices(m_modelMatrices);
	m_rasterizer->updateTextureLayerIDs(m_textureIDs);

	//LOG(INFO) << "Add instanced object " << textureID;
}

// ------------------------------------------------------------------------
void InstancedObject::removeInstancedObject(const unsigned int index)
{
	if (index < m_modelMatrices.size())
	{
		// Remove model matrix and texture ID
		m_modelMatrices.erase(m_modelMatrices.begin() + index);
		m_textureIDs.erase(m_textureIDs.begin() + index);

		// Update instance buffers
		m_rasterizer->updateModelMatrices(m_modelMatrices);
		m_rasterizer->updateTextureLayerIDs(m_textureIDs);

		//LOG(INFO) << "Instanced object removed at index " << index;
	}
}

// ------------------------------------------------------------------------
void InstancedObject::renderObject(const std::shared_ptr<Shader> shaderPtr, const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
	// Draw the instanced object
	m_rasterizer->drawArrays(m_VAO, m_textureIDs.size());
}
