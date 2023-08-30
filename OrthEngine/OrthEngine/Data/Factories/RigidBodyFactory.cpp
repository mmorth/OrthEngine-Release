#include "RigidBodyFactory.hpp"

// ------------------------------------------------------------------------
RigidBodyFactory& RigidBodyFactory::getInstance()
{
	static RigidBodyFactory instance;
	return instance;
}

// ------------------------------------------------------------------------
RigidBodyFactory::RigidBodyFactory()
{}

// ------------------------------------------------------------------------
std::optional<std::unique_ptr<RigidBody>> RigidBodyFactory::createRigidBody(const ObjectConfig& objectConfig)
{
	std::optional<std::unique_ptr<RigidBody>> rigidBodyOpt = std::nullopt;
	switch (objectConfig.physicsShapeInfo.shapeType)
	{
		case ShapeType::Box:
			rigidBodyOpt = std::make_unique<RigidBody>(objectConfig.physicsShapeInfo, objectConfig.objectLocationOrientation);
			break;
		default:
			return std::nullopt;
	}

	return rigidBodyOpt;
}