#include "RenderObjectSceneCreator.hpp"

namespace
{
	std::array<float, MathUtils::MAT3_SIZE> MAT3_IDENTITY_MATRIX
	{
		1, 0, 0,
		0, 1, 0,
		0, 0, 1
	};
}

// ------------------------------------------------------------------------
RenderObjectSceneCreator::RenderObjectSceneCreator()
	: m_numInstancedObjects()
{}

// ------------------------------------------------------------------------
RenderObjectSceneCreator::~RenderObjectSceneCreator()
{}

// ------------------------------------------------------------------------
std::vector<ObjectConfig> RenderObjectSceneCreator::createExperimentalScene()
{
	// Define a struct list that will return the config of all objects to be created
	std::vector<ObjectConfig> renderObjectConfigList{};

	// TODO: Evenutally the ObjectConfig variables will be in the UIApp
	// ===== Ground Plane =====
	// Instanced Cube Objects
	// Note: All instanced cubes should have the same texture array and just select which textures they want from the array
	ObjectConfig instancedCubeObjectConfig;
	instancedCubeObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_CUBE, false, true };
	instancedCubeObjectConfig.objectMaterialNames = { 32.0f, "Instanced_Texture_Array", "container2_specular.PNG" };
	instancedCubeObjectConfig.physicsShapeInfo = { ShapeType::Box, { 0.0f, 1.0f } };

	// Create bottom plane
	drawInstancedPlane(PlaneDimension::Y_AXIS, AxisPlaneDrawInfo(MathUtils::Vec2(-5.0f, 5.0f), MathUtils::Vec2(-5.0f, 5.0f), -2.0f), instancedCubeObjectConfig.instancedObjectProperties.modelMat, instancedCubeObjectConfig.instancedObjectProperties.textureIDs);
	// Create left wall
	drawInstancedPlane(PlaneDimension::X_AXIS, AxisPlaneDrawInfo(MathUtils::Vec2(-1.0f, 5.0f), MathUtils::Vec2(-5.0f, 5.0f), -5.0f), instancedCubeObjectConfig.instancedObjectProperties.modelMat, instancedCubeObjectConfig.instancedObjectProperties.textureIDs);
	// Create right wall
	drawInstancedPlane(PlaneDimension::X_AXIS, AxisPlaneDrawInfo(MathUtils::Vec2(-1.0f, 5.0f), MathUtils::Vec2(-5.0f, 5.0f), 4.0f), instancedCubeObjectConfig.instancedObjectProperties.modelMat, instancedCubeObjectConfig.instancedObjectProperties.textureIDs);
	// Create back wall
	drawInstancedPlane(PlaneDimension::Z_AXIS, AxisPlaneDrawInfo(MathUtils::Vec2(-5.0f, 5.0f), MathUtils::Vec2(-1.0f, 5.0f), 5.0f), instancedCubeObjectConfig.instancedObjectProperties.modelMat, instancedCubeObjectConfig.instancedObjectProperties.textureIDs);
	// Create top plane
	drawInstancedPlane(PlaneDimension::Y_AXIS, AxisPlaneDrawInfo(MathUtils::Vec2(-5.0f, 5.0f), MathUtils::Vec2(-5.0f, 5.0f), 5.0f), instancedCubeObjectConfig.instancedObjectProperties.modelMat, instancedCubeObjectConfig.instancedObjectProperties.textureIDs);
	renderObjectConfigList.push_back(instancedCubeObjectConfig);

	// ===== Non Instanced Objects =====
	// Player
	ObjectConfig playerObjectConfig;
	MathUtils::Vec3 playerOrigin(0.0f, 0.0f, 0.0f);
	playerObjectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_CUBE, true, true };
	playerObjectConfig.objectLocationOrientation = { playerOrigin, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
	playerObjectConfig.objectMaterialNames = { 32.0f, "container2_resized.PNG", "container2_specular.PNG" };
	playerObjectConfig.physicsShapeInfo = { ShapeType::Box, { 1.0f, 0.0f } };
	renderObjectConfigList.push_back(playerObjectConfig);


	// Cube Object
	ObjectConfig cubeObjectConfig;
	MathUtils::Vec3 cubeOrigin(-1.0f, -1.0f, -2.0f);
	cubeObjectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_CUBE, false, true };
	cubeObjectConfig.objectLocationOrientation = { cubeOrigin, MathUtils::Vec3{1.0f, 1.0f, 1.0f}, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
	cubeObjectConfig.objectMaterialNames = { 32.0f, "container2_resized.PNG", "container2_specular.PNG" };
	cubeObjectConfig.physicsShapeInfo = { ShapeType::Box, { 0.0f, 1.0f } };
	renderObjectConfigList.push_back(cubeObjectConfig);

	// TODO: Convert these over to instanced rendering
	// ===== Transparent Objects =====
	// // Grass
	ObjectConfig grassObjectConfig;
	grassObjectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_PLANE, false, false };
	grassObjectConfig.objectLocationOrientation = { MathUtils::Vec3{ 1.0f, -1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
	grassObjectConfig.objectMaterialNames = { 32.0f, "grass.png", "grass.png" };
	renderObjectConfigList.push_back(grassObjectConfig);

	// Window
	ObjectConfig windowObjectConfig;
	windowObjectConfig.renderObjectProperties = { GeometryTypes::NONINSTANCED_PLANE, false, false };
	windowObjectConfig.objectLocationOrientation = { MathUtils::Vec3{ 0.0f, -1.0f, -2.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
	windowObjectConfig.objectMaterialNames = { 32.0f, "blending_transparent_window_resized.png", "blending_transparent_window_resized.png" };
	renderObjectConfigList.push_back(windowObjectConfig);


	// ===== Light Objects =====
	// Directional Light
	ObjectConfig directionalLightConfig;
	directionalLightConfig.renderObjectProperties = { GeometryTypes::DIRECTIONAL_LIGHT, false, false };
	directionalLightConfig.lightProperties = { MathUtils::Vec3{ 0.0f, -1.0f, 0.0f }, MathUtils::Vec3{ 0.0f, -1.0f, 0.0f } };
	directionalLightConfig.phongLightProperties = { { 0.05f, 0.05f, 0.05f }, { 0.4f, 0.4f, 0.4f }, { 0.5f, 0.5f, 0.5f } };
	renderObjectConfigList.push_back(directionalLightConfig);


	// Spot Light
	ObjectConfig spotLightConfig;
	spotLightConfig.renderObjectProperties = { GeometryTypes::INSTANCED_SPOT_LIGHT, false, false };
	spotLightConfig.lightProperties = { MathUtils::Vec3{ 0.0f, 0.0f, 0.0f }, MathUtils::Vec3{ 0.0f, 0.0f, 0.0f } };
	spotLightConfig.phongLightProperties = { {0.0f, 0.0f, 0.0f}, { 1.0f, 1.0f, 1.0f }, { 1.0f, 1.0f, 1.0f } };
	spotLightConfig.attenuationParams = { 1.0f, 0.09f, 0.032f };
	spotLightConfig.spotlightCutoffParams = { 12.5f, 15.0f };
	renderObjectConfigList.push_back(spotLightConfig);


	// Point Light
	ObjectConfig pointLightObjectConfig;
	pointLightObjectConfig.renderObjectProperties = { GeometryTypes::INSTANCED_POINT_LIGHT, false, true };
	pointLightObjectConfig.objectMaterialNames = { 32.0f, "Instanced_Texture_Array", "container2_specular.PNG" };
	pointLightObjectConfig.physicsShapeInfo = { ShapeType::Box, { 0.0f, 1.0f } };
	pointLightObjectConfig.phongLightProperties = { {0.5f, 0.5f, 0.5f}, { 0.8f, 0.8f, 0.8f }, { 1.0f, 1.0f, 1.0f } };
	pointLightObjectConfig.attenuationParams = { 1.0f, 0.09f, 0.032f };

	// Add instanced cube model matrix attribs
	ObjectProperties tmpObjProps{ MathUtils::Vec3{1.0f, 1.0f, -2.0f}, MathUtils::Vec3{1.0f, 1.0f, 1.0f}, 0.0f };

	Transform transform{};
	transform.resetMatrix(MatrixTransform::MatrixTypes::MODEL);
	transform.applyTranslation(MatrixTransform::MatrixTypes::MODEL, tmpObjProps.position);
	transform.applyScale(MatrixTransform::MatrixTypes::MODEL, tmpObjProps.size);
	transform.applyRotation(MatrixTransform::MatrixTypes::MODEL, tmpObjProps.orientation, tmpObjProps.size);
	std::array<float, MathUtils::MAT4_SIZE> modelMat = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::MODEL);
	pointLightObjectConfig.instancedObjectProperties.modelMat.push_back(modelMat);

	pointLightObjectConfig.instancedObjectProperties.textureIDs.push_back(-1.0f);
	renderObjectConfigList.push_back(pointLightObjectConfig);

	// ===== Text Object =====
	ObjectConfig textConfig;
	textConfig.renderObjectProperties = { GeometryTypes::TEXT, false, false };
	textConfig.textProperties = { "Sample Text", {50.0f, 50.0f}, 2.0f, {1.0f, 1.0f, 1.0f} };
	renderObjectConfigList.push_back(textConfig);

	// ===== Skybox Object =====
	ObjectConfig skyboxConfig;
	skyboxConfig.renderObjectProperties = { GeometryTypes::SKYBOX, false, false };
	renderObjectConfigList.push_back(skyboxConfig);

	LOG(INFO) << "Experimental scene created";

	return renderObjectConfigList;
}

void RenderObjectSceneCreator::drawInstancedPlane(const PlaneDimension& planeDimension, const AxisPlaneDrawInfo& axisPlaneDrawInfo, std::vector< std::array<float, MathUtils::MAT4_SIZE>>& modelMatVec, std::vector<float>& textureIDsVec)
{
	// Create bottom plane
	for (float i = axisPlaneDrawInfo.axis1Range.x; i < axisPlaneDrawInfo.axis1Range.y; i += 1.0f)
	{
		for (float j = axisPlaneDrawInfo.axis2Range.x; j < axisPlaneDrawInfo.axis2Range.y; j += 1.0f)
		{
			// Add instanced cube model matrix attribs
			ObjectProperties tmpObjProps;
			switch (planeDimension)
			{
				case PlaneDimension::X_AXIS:
					tmpObjProps = { MathUtils::Vec3{ axisPlaneDrawInfo.fixedAxisPosition, i, j }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
					break;

				case PlaneDimension::Y_AXIS:
					tmpObjProps = { MathUtils::Vec3{ i, axisPlaneDrawInfo.fixedAxisPosition, j }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
					break;

				case PlaneDimension::Z_AXIS:
					tmpObjProps = { MathUtils::Vec3{ i, j, axisPlaneDrawInfo.fixedAxisPosition }, MathUtils::Vec3{ 1.0f, 1.0f, 1.0f }, 0.0f };
					break;

				default:
					break;
			}

			Transform transform{};
			transform.resetMatrix(MatrixTransform::MatrixTypes::MODEL);
			transform.applyTranslation(MatrixTransform::MatrixTypes::MODEL, tmpObjProps.position);
			transform.applyScale(MatrixTransform::MatrixTypes::MODEL, tmpObjProps.size);
			transform.applyRotation(MatrixTransform::MatrixTypes::MODEL, tmpObjProps.orientation, tmpObjProps.position);
			std::array<float, MathUtils::MAT4_SIZE> modelMat = transform.getTransformationMatrix(MatrixTransform::MatrixTypes::MODEL);
			modelMatVec.push_back(modelMat);

			// Add instanced cube texture layer attrib
			m_numInstancedObjects++;
			if (m_numInstancedObjects < 51) // TODO: Make this more flexible
				textureIDsVec.push_back(1.0f);
			else
				textureIDsVec.push_back(0.0f);
		}
	}
}
