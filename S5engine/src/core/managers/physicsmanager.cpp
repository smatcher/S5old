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

	// Forme en tant que boite
	btCollisionShape* shape_sol = new btBoxShape( btVector3(10,1,10) );

	btTransform myTransform;
	myTransform.setIdentity();

	// Position du sol
	myTransform.setOrigin( btVector3(0,-2,0) );
	btVector3 localInertiaSol(0,0,0);

	btScalar mass = 0; // Le fait que le poids de cet objet soit zéro le rends statique

	btMotionState* myMotionState_Sol = new btDefaultMotionState(myTransform);

	btRigidBody::btRigidBodyConstructionInfo sol_info( mass, myMotionState_Sol, shape_sol, localInertiaSol );

	btRigidBody* body_sol = new btRigidBody(sol_info);

	// On ajoute le sol dans le monde Bullet
	m_discreteDynamicsWorld->addRigidBody(body_sol);

	m_debug = new GLDebugDrawer();
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

	for(QVector<PhysicObject*>::iterator it = registeredManagees.begin();
		it != registeredManagees.end();
		it++) {
		(*it)->onPhysicUpdate();
	}
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
