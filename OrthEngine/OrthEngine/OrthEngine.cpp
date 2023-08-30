#include <cmath>
#include <iostream>
#include <string>

#include "OrthEngine.hpp"

//class ContactCallback : public btCollisionWorld::ContactResultCallback
//{
//private:
//	btRigidBody* m_playerRigidBody;
//
//public:
//	ContactCallback(btRigidBody* playerRigidBody) : m_playerRigidBody(playerRigidBody) {}
//
//	virtual btScalar addSingleResult(btManifoldPoint& cp,
//		const btCollisionObjectWrapper* colObj0Wrap, int partId0, int index0,
//		const btCollisionObjectWrapper* colObj1Wrap, int partId1, int index1) override
//	{
//		// Ensure that one of the collision objects is the player's rigid body
//		if (colObj0Wrap->getCollisionObject() == m_playerRigidBody ||
//			colObj1Wrap->getCollisionObject() == m_playerRigidBody)
//		{
//			// Get the minimum translation vector (MTV) from the contact point
//			btVector3 normal = cp.m_normalWorldOnB;
//			btScalar penetrationDepth = cp.m_distance1;
//			btVector3 mtv = normal * penetrationDepth;
//
//			// Calculate the new position of the player to avoid collision
//			btVector3 playerPosition = m_playerRigidBody->getWorldTransform().getOrigin();
//			btVector3 newPosition = playerPosition - mtv;
//
//			// Update the player's position
//			btTransform newTransform = m_playerRigidBody->getWorldTransform();
//			newTransform.setOrigin(newPosition);
//			m_playerRigidBody->setWorldTransform(newTransform);
//
//			// Print the details
//			std::cout << "==============================\n";
//			std::cout << "===== COLLISION DETECTED =====\n";
//			std::cout << "MTV: (" << mtv.x() << ", " << mtv.y() << ", " << mtv.z() << ")\n";
//			std::cout << "Player Position After Collision: (" << newPosition.x() << ", " << newPosition.y() << ", " << newPosition.z() << ")\n";
//			std::cout << "==============================\n";
//		}
//
//		return 0;
//	}
//};

int main()
{
	//// Create physics world
	//btDefaultCollisionConfiguration* m_collisionConfiguration = new btDefaultCollisionConfiguration();
	//btCollisionDispatcher* m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	//btDbvtBroadphase* m_broadphase = new btDbvtBroadphase();
	//btSequentialImpulseConstraintSolver* m_solver = new btSequentialImpulseConstraintSolver();
	//btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_broadphase, m_solver, m_collisionConfiguration);
	//dynamicsWorld->setGravity(btVector3(0, 0, 0));
	//
	//// Create player rigid body
	//btBoxShape* playerShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	//btTransform playerTransform; playerTransform.setIdentity();
	//playerTransform.setOrigin(btVector3(0.0f, 0.0f, 0.0f));

	//btVector3 playerLocalIntertia;
	//playerShape->calculateLocalInertia(1.0f, playerLocalIntertia);
	//btDefaultMotionState* playerMotionState = new btDefaultMotionState(playerTransform);
	//btRigidBody::btRigidBodyConstructionInfo playerInfo(1.0f, playerMotionState, playerShape, playerLocalIntertia);
	//btRigidBody* player = new btRigidBody(playerInfo);
	//player->setFriction(0.0f);

	//// Create cube rigid body
	//btBoxShape* cubeShape = new btBoxShape(btVector3(0.5f, 0.5f, 0.5f));
	//btTransform cubeTransform; cubeTransform.setIdentity();
	//cubeTransform.setOrigin(btVector3(5.0f, 5.0f, 5.0f));

	//btVector3 cubeLocalIntertia;
	//cubeShape->calculateLocalInertia(1.0f, cubeLocalIntertia);
	//btDefaultMotionState* cubeMotionState = new btDefaultMotionState(cubeTransform);
	//btRigidBody::btRigidBodyConstructionInfo cubeInfo(1.0f, cubeMotionState, cubeShape, cubeLocalIntertia);
	//btRigidBody* cube = new btRigidBody(cubeInfo);
	//cube->setFriction(0.0f);

	//// Add player and cube to physics world
	//dynamicsWorld->addRigidBody(player);
	//dynamicsWorld->addRigidBody(cube);

	//// Update player position
	//// Calculate the velocity needed to reach the target position within a certain time frame
	//btVector3 targetPosition(4.1f, 5.0f, 5.0f);
	//btVector3 currentPosition = player->getWorldTransform().getOrigin();
	//btVector3 displacement = targetPosition - currentPosition;
	//float timeToReach = 1.0f; // Adjust this value based on your desired time to reach the target
	//btVector3 desiredVelocity = displacement / timeToReach;
	//std::cout << "Initial Player Position: (" << currentPosition.x() << ", " << currentPosition.y() << ", " << currentPosition.z() << ")\n";
	//btVector3 initialVelocity = player->getLinearVelocity();
	//std::cout << "Initial Player Velocity: (" << initialVelocity.x() << ", " << initialVelocity.y() << ", " << initialVelocity.z() << ")\n";
	//std::cout << "Desired Player Velocity: (" << desiredVelocity.x() << ", " << desiredVelocity.y() << ", " << desiredVelocity.z() << ")\n";
	//player->setLinearVelocity(desiredVelocity);

	//dynamicsWorld->performDiscreteCollisionDetection();
	//dynamicsWorld->stepSimulation(1.0f, 0.0f);

	//currentPosition = player->getWorldTransform().getOrigin();
	//btVector3 finalVelocity = player->getLinearVelocity();
	//std::cout << "Final Player Position: (" << currentPosition.x() << ", " << currentPosition.y() << ", " << currentPosition.z() << ")\n";
	//std::cout << "Final Player Velocity: (" << finalVelocity.x() << ", " << finalVelocity.y() << ", " << finalVelocity.z() << ")\n";

	//ContactCallback contactCallback(player);
	//dynamicsWorld->contactPairTest(player, cube, contactCallback);

	//// Confirm that the MVT is correctly applied to the bullet simulation
	//btVector3 targetPosition2(4.1f, 5.0f, 5.0f);
	//btVector3 currentPosition2 = player->getWorldTransform().getOrigin();
	//btVector3 displacement2 = targetPosition2 - currentPosition2;
	//float timeToReach2 = 1.0f; // Adjust this value based on your desired time to reach the target
	//btVector3 desiredVelocity2 = displacement2 / timeToReach2;
	//std::cout << "Initial Player Position2: (" << currentPosition2.x() << ", " << currentPosition2.y() << ", " << currentPosition2.z() << ")\n";
	//btVector3 initialVelocity2 = player->getLinearVelocity();
	//std::cout << "Initial Player Velocity2: (" << initialVelocity2.x() << ", " << initialVelocity2.y() << ", " << initialVelocity2.z() << ")\n";
	//std::cout << "Desired Player Velocity2: (" << desiredVelocity2.x() << ", " << desiredVelocity2.y() << ", " << desiredVelocity2.z() << ")\n";
	//player->setLinearVelocity(desiredVelocity2);
	//btVector3 setVelocity2 = player->getLinearVelocity();
	//std::cout << "Set Player Velocity2: (" << setVelocity2.x() << ", " << setVelocity2.y() << ", " << setVelocity2.z() << ")\n";

	//dynamicsWorld->performDiscreteCollisionDetection();
	//dynamicsWorld->stepSimulation(1.0f, 0.0f);

	//currentPosition2 = player->getWorldTransform().getOrigin();
	//btVector3 finalVelocity2 = player->getLinearVelocity();
	//std::cout << "Final Player Position2: (" << currentPosition2.x() << ", " << currentPosition2.y() << ", " << currentPosition2.z() << ")\n";
	//std::cout << "Final Player Velocity2: (" << finalVelocity2.x() << ", " << finalVelocity2.y() << ", " << finalVelocity2.z() << ")\n";

	//dynamicsWorld->contactPairTest(player, cube, contactCallback);

	//// Remove rigid bodies
	//dynamicsWorld->removeRigidBody(player);
	//dynamicsWorld->removeRigidBody(cube);

	//delete player;
	//delete playerMotionState;
	//delete playerShape;

	//delete cube;
	//delete cubeMotionState;
	//delete cubeShape;

	//delete dynamicsWorld;
	//delete m_solver;
	//delete m_broadphase;
	//delete m_dispatcher;
	//delete m_collisionConfiguration;




	//// Create Physics World and Rigid Bodies
	//std::unique_ptr<PhysicsWorld> physicsWorld = std::make_unique<PhysicsWorld>();
	//MathUtils::Vec3 loc(0.0f, 0.0f, 0.0f);
	////MathUtils::Vec3 loc(5.0f, 5.0f, 5.0f);
	//RigidBodyConfig playerConfig{ ShapeInfo(), 1.0f, PhysicsTransform(loc, MathUtils::Vec3(1.0f, 1.0f, 1.0f), 0.0f) };
	//std::unique_ptr<RigidBody> player = std::make_unique<RigidBody>(playerConfig.shapeInfo, playerConfig.mass, playerConfig.physicsTransform);
	//MathUtils::Vec3 pos(5.0f, 5.0f, 5.0f);
	////MathUtils::Vec3 pos(-0.5f, -0.5f, -0.5f);
	//RigidBodyConfig otherConfig{ ShapeInfo(), 1.0f, PhysicsTransform(pos, MathUtils::Vec3(1.0f, 1.0f, 1.0f), 0.0f) };
	//std::unique_ptr<RigidBody> other = std::make_unique<RigidBody>(otherConfig.shapeInfo, otherConfig.mass, otherConfig.physicsTransform);

	//physicsWorld->addRigidBody(player.get());
	//physicsWorld->addRigidBody(other.get());
	//
	//player->moveToPosition(MathUtils::Vec3(5.0f, 5.0f, 5.0f), 1.0f);

	//physicsWorld->stepSimulation(1.0f, true);
	//physicsWorld->detectCollision(player.get(), other.get(), CollisionType::CONTACT_STOP);

	//player->moveToPosition(MathUtils::Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	//physicsWorld->stepSimulation(1.0f, true);
	//physicsWorld->detectCollision(player.get(), other.get(), CollisionType::CONTACT_STOP);

	//player->moveToPosition(MathUtils::Vec3(0.0f, 0.0f, 0.0f), 1.0f);

	//physicsWorld->stepSimulation(1.0f, true);
	//physicsWorld->detectCollision(player.get(), other.get(), CollisionType::CONTACT_STOP);

	//physicsWorld->removeRigidBody(player.get());
	//physicsWorld->removeRigidBody(other.get());

		


	// Configure g3log
	auto worker = g3::LogWorker::createLogWorker();
	auto loggerHandle = worker->addDefaultLogger("global_logger", "./logs"); // Provide a unique logger ID and the log directory

	// Initialize g3log
	g3::initializeLogging(worker.get());

	// Create and start EngineApp
	EngineApp app;
	app.run();

	LOG(INFO) << "Application Start";

	return 0;
}
