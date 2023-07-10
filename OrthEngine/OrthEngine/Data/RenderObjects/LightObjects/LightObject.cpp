#include "LightObject.hpp"

// ------------------------------------------------------------------------
LightObject::LightObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<InstancedRasterizer> rasterizer, const PhongLightingParams& phongLightProperties)
	: InstancedObject(shaderPtr, rasterizer)
	, m_phongLightParams(phongLightProperties)
{
	LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
LightObject::~LightObject()
{
	LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void LightObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
	InstancedObject::render(projectionMatrix, viewMatrix);

	LOG(INFO) << "LightObject Render";
}
