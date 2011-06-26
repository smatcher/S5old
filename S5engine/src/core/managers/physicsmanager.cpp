#include "core/managers/physicsmanager.h"
#include "core/properties/physicobject.h"
#include "core/utils/GLDebugDrawer.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

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
	//m_debug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
	m_debug->setDebugMode(btIDebugDraw::DBG_DrawAabb);
	m_discreteDynamicsWorld->setDebugDrawer(m_debug);
}

PhysicsManager::~PhysicsManager()
{
	delete m_discreteDynamicsWorld;
	delete m_broadphaseInterface;
	delete m_collisionDispatcher;
	delete m_collisionConfiguration;
	delete m_sequentialImpulseConstraintSolver;

	for(QMap<QString,btCollisionShape*>::iterator it = m_colliders.begin() ;
		it != m_colliders.end() ; it++) {
		delete *it;
	}
}

void PhysicsManager::buildConvexCollider(QString name, float* vertices, int nb_vertices)
{
	btConvexHullShape* shape = new btConvexHullShape(vertices, nb_vertices, 3*sizeof(float));
	//shape->setMargin(0);

	if(nb_vertices > 42) {
		btShapeHull* shape_hull = new btShapeHull(shape);
		btConvexHullShape* old_shape = shape;
		shape_hull->buildHull(old_shape->getMargin());
		shape = new btConvexHullShape((btScalar*)shape_hull->getVertexPointer(), shape_hull->numVertices());
		delete shape_hull;
		delete old_shape;
	}

	m_colliders.insert(name, shape);
}

btCollisionShape* PhysicsManager::getCollider(QString name)
{
	return m_colliders.value(name, NULL);
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
