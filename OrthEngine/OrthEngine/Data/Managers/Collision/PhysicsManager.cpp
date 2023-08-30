#include "PhysicsManager.hpp"

// TODO: Determine how to include instead of hard coding this value
# define M_PI           3.14159265358979323846

// ------------------------------------------------------------------------
PhysicsManager::PhysicsManager()
	: m_physicsWorld(std::make_shared<PhysicsWorld>())
	, m_rigidBodyFactory(std::make_shared<RigidBodyFactory>(RigidBodyFactory::getInstance()))
	, m_rigidBodies()
	, m_playerID(0)
{
	LOG(INFO) << "ctor";
}

// ------------------------------------------------------------------------
PhysicsManager::~PhysicsManager()
{
	LOG(INFO) << "dtor";

	cleanup();
}

// ------------------------------------------------------------------------
void PhysicsManager::initialize(const std::shared_ptr<PhysicsWorld>& physicsWorld, const std::shared_ptr<RigidBodyFactory>& rigidBodyFactory)
{
	if (physicsWorld != nullptr && rigidBodyFactory != nullptr)
	{
		m_physicsWorld = physicsWorld;
		m_rigidBodyFactory = rigidBodyFactory;
		LOG(INFO) << "Initialize PhysicsManager";
	}
}

// ------------------------------------------------------------------------
void PhysicsManager::cleanup()
{
	for (auto it = m_rigidBodies.begin(); it != m_rigidBodies.end(); )
	{
		for (const auto& rigidBody : it->second)
			m_physicsWorld->removeRigidBody(rigidBody.get());

		it = m_rigidBodies.erase(it);
	}
}

// ------------------------------------------------------------------------
void PhysicsManager::createRigidBody(const int id, ObjectConfig& objectConfig, bool isPlayer)
{
	// TODO: Implement
	LOG(INFO) << "Create rigid body " << +id;

	std::optional<std::unique_ptr<RigidBody>> rigidBodyOpt = RigidBodyFactory::getInstance().createRigidBody(objectConfig);
	if (!rigidBodyOpt.has_value())
		return;

	// Create instanced objects if exists
	std::vector<std::unique_ptr<RigidBody>> rigidBodies;
	for (std::size_t index = 0; index < objectConfig.instancedObjectProperties.textureIDs.size(); index++)
	{
		LOG(INFO) << "Instanced object created " << index;

		// Compute ObjectLocationOrientation properties from moodel matrix
		std::array<float, MathUtils::MAT4_SIZE> curModelMat = objectConfig.instancedObjectProperties.modelMat.at(index);
		MathUtils::Vec3 location(curModelMat.at(12), curModelMat.at(13), curModelMat.at(14));
		MathUtils::Vec3 scale(curModelMat.at(0), curModelMat.at(5), curModelMat.at(10));

		// Extract the rotation matrix (upper-left 3x3 submatrix)
		std::array<MathUtils::Vec3, 3> rotationMatrix;
		for (int i = 0; i < 3; ++i) 
		{
			rotationMatrix[i].x = curModelMat[i * 4];
			rotationMatrix[i].y = curModelMat[i * 4 + 1];
			rotationMatrix[i].z = curModelMat[i * 4 + 2];
		}

		// Compute and normalize the axis of rotation
		MathUtils::Vec3 axis;
		axis.x = rotationMatrix[2].y - rotationMatrix[1].z;
		axis.y = rotationMatrix[0].z - rotationMatrix[2].x;
		axis.z = rotationMatrix[1].x - rotationMatrix[0].y;
		axis.normalize();

		// Calculate the angle of rotation
		float trace = rotationMatrix[0].x + rotationMatrix[1].y + rotationMatrix[2].z;
		float angle = std::acos((trace - 1.0f) * 0.5f);
		angle = angle * 180.0f / M_PI;

		objectConfig.objectLocationOrientation = ObjectLocationOrientation( location, scale, axis, angle );

		std::optional<std::unique_ptr<RigidBody>> rigidBodyOpt = RigidBodyFactory::getInstance().createRigidBody(objectConfig);
		if (rigidBodyOpt.has_value())
		{
			m_physicsWorld->addRigidBody(rigidBodyOpt.value().get());
			rigidBodies.push_back(std::move(rigidBodyOpt.value()));
		}
	}

	// Create non instanced rigid body if exists
	if (objectConfig.renderObjectProperties.geometryType == GeometryTypes::NONINSTANCED_CUBE && objectConfig.renderObjectProperties.isCollidable)
	{
		std::optional<std::unique_ptr<RigidBody>> rigidBodyOpt = RigidBodyFactory::getInstance().createRigidBody(objectConfig);
		if (rigidBodyOpt.has_value())
		{
			m_physicsWorld->addRigidBody(rigidBodyOpt.value().get());
			rigidBodies.push_back(std::move(rigidBodyOpt.value()));
		}
	}

	// Add list of rigid bodies to map
	m_rigidBodies.emplace(id, std::move(rigidBodies));

	if (isPlayer)
		m_playerID = id;
}

// ------------------------------------------------------------------------
void PhysicsManager::removeRigidBodyWithID(const unsigned int rigidBodyID, const unsigned int instanceID)
{
	LOG(INFO) << "Remove rigid body " << +rigidBodyID;

	auto rigidBodiesToDelete = m_rigidBodies.find(m_playerID);
	if (rigidBodiesToDelete != m_rigidBodies.end())
	{
		if (instanceID >= 0 && instanceID < rigidBodiesToDelete->second.size())
		{
			rigidBodiesToDelete->second.erase(rigidBodiesToDelete->second.begin() + instanceID);

			if (rigidBodiesToDelete->second.size() == 0)
				m_rigidBodies.erase(rigidBodyID);
		}
	}
}

// ------------------------------------------------------------------------
MathUtils::Vec3 PhysicsManager::getPlayerLocation()
{
	auto playerRigidBody = m_rigidBodies.find(m_playerID);
	return playerRigidBody->second.at(0)->getPosition();
}

// ------------------------------------------------------------------------
void PhysicsManager::updatePlayerLocation(const MathUtils::Vec3& newPlayerLocation, const float timeToReach)
{
	auto playerRigidBody = m_rigidBodies.find(m_playerID);
	playerRigidBody->second.at(0)->moveToPosition(newPlayerLocation, timeToReach);
}

// ------------------------------------------------------------------------
MathUtils::Vec3 PhysicsManager::detectCameraCollision(const MathUtils::Vec3& playerPosition, const MathUtils::Vec3& cameraPosition, const MathUtils::Vec3& cameraFront)
{
	return m_physicsWorld->detectCameraCollision(playerPosition, cameraPosition, cameraFront);
}

// ------------------------------------------------------------------------
std::optional<MathUtils::Vec3> PhysicsManager::stepSimulationAndCheckCollision(const float deltaTimeSec)
{
	// Step the simulation
	m_physicsWorld->stepSimulation(deltaTimeSec, true);

	// Return the new camera position
	return std::nullopt;
}
