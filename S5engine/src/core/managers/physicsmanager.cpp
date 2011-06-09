#include "core/managers/physicsmanager.h"
#include "core/properties/physicobject.h"
#include "core/utils/GLDebugDrawer.h"

#include "btBulletDynamicsCommon.h"

PhysicsManager::PhysicsManager()
{
	m_broadphaseInterface = new btDbvtBroadphase();
	m_collisionConfiguration = new btDefaultCollisionConfiguration();
	m_collisionDispatcher = new btCollisionDispatcher(m_collisionConfiguration);
	m_sequentialImpulseConstraintSolver = new btSequentialImpulseConstraintSolver();
	m_discreteDynamicsWorld = new btDiscreteDynamicsWorld(
		m_collisionDispatcher,
		m_broadphaseInterface,
		m_sequentialImpulseConstraintSolver,
		m_collisionConfiguration
	);
	m_discreteDynamicsWorld->setGravity(btVector3(0,-9.81,0));

	m_debug = new GLDebugDrawer();
	m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_discreteDynamicsWorld->setDebugDrawer(m_debug);
}

PhysicsManager::~PhysicsManager()
{
	delete m_discreteDynamicsWorld;
	delete m_broadphaseInterface;
	delete m_collisionDispatcher;
	delete m_collisionConfiguration;
	delete m_sequentialImpulseConstraintSolver;
}

void PhysicsManager::update(double elapsed)
{
	if(m_discreteDynamicsWorld != NULL)
		m_discreteDynamicsWorld->stepSimulation(elapsed);
}

void PhysicsManager::onManageeLinked(PhysicObject *managee)
{
	if(m_discreteDynamicsWorld != NULL && managee != NULL) {
		m_discreteDynamicsWorld->addRigidBody(managee->getRigidBody());
	}
}

void PhysicsManager::debugDraw()
{
	if(m_discreteDynamicsWorld != NULL)
		m_discreteDynamicsWorld->debugDrawWorld();
}
