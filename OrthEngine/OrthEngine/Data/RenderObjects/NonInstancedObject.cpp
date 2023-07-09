#include "NonInstancedObject.hpp"

// ------------------------------------------------------------------------
NonInstancedObject::NonInstancedObject(const std::shared_ptr<Shader> shaderPtr, const std::shared_ptr<NonInstancedRasterizer> rasterizerPtr, const ObjectLocation& objectLocation)
    : RenderObject(shaderPtr)
    , m_rasterizer(rasterizerPtr)
    , m_objectLocation(objectLocation)
    , m_transform(std::make_shared<Transform>())
    , m_VAO(m_rasterizer->createNewVAO())
{
    //LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
NonInstancedObject::~NonInstancedObject()
{
    //LOG(INFO) << "dtor";
}

// ------------------------------------------------------------------------
void NonInstancedObject::render(const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
	// Render object with default object shader
    RenderObject::render(projectionMatrix, viewMatrix);
    renderObject(m_shader, projectionMatrix, viewMatrix);

    //LOG(INFO) << "NonInstancedObject Render";
}

// ------------------------------------------------------------------------
void NonInstancedObject::drawNormals(const std::shared_ptr<Shader> shaderPtr, std::array<float, MathUtils::MAT4_SIZE> projectionMatrix, std::array<float, MathUtils::MAT4_SIZE> viewMatrix)
{
    // Render object with default object shader
    RenderObject::drawNormals(shaderPtr, projectionMatrix, viewMatrix);
    renderObject(shaderPtr, projectionMatrix, viewMatrix);

    //LOG(INFO) << "Render NonInstancedObject Normals";
}

// ------------------------------------------------------------------------
void NonInstancedObject::updateLocation(const MathUtils::Vec3& newPosition)
{
    m_objectLocation.position = newPosition;
}

// ------------------------------------------------------------------------
void NonInstancedObject::renderObject(const std::shared_ptr<Shader> shaderPtr, const std::array<float, MathUtils::MAT4_SIZE>& projectionMatrix, const std::array<float, MathUtils::MAT4_SIZE>& viewMatrix)
{
    // Apply the model matrix transformation
    shaderPtr->use();
    m_transform->resetMatrix(MatrixTransform::MatrixTypes::MODEL);
    m_transform->applyTranslation(MatrixTransform::MatrixTypes::MODEL, m_objectLocation.position);
    m_transform->applyScale(MatrixTransform::MatrixTypes::MODEL, m_objectLocation.size);
    m_transform->applyRotation(MatrixTransform::MatrixTypes::MODEL, m_objectLocation.orientation, m_objectLocation.position);
    std::array<float, MathUtils::MAT4_SIZE> modelMatrix = m_transform->getTransformationMatrix(MatrixTransform::MatrixTypes::MODEL);
    shaderPtr->setMat4("model", modelMatrix.data());

    m_rasterizer->drawArrays(m_VAO);
}

// ------------------------------------------------------------------------
void NonInstancedObject::setTransform(const std::shared_ptr<Transform> transform)
{
    m_transform = transform;
}
