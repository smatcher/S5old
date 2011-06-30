#include "core/managers/physicsmanager.h"
#include "core/properties/physicobject.h"
#include "core/utils/GLDebugDrawer.h"

#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"
#include "BulletCollision/CollisionShapes/btConvexPointCloudShape.h"
#include "LinearMath/btGeometryUtil.h"

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

	for(QMap<QString,btCollisionShape*>::iterator it = m_concave_colliders.begin() ;
		it != m_concave_colliders.end() ; it++) {
		delete *it;
	}
	for(QMap<QString,btConvexPointCloudShape*>::iterator it = m_convex_colliders.begin() ;
		it != m_convex_colliders.end() ; it++) {
		delete *it;
	}
}

void PhysicsManager::buildConvexCollider(QString name, float* vertices, int nb_vertices)
{
	float collisionMargin = 0.04f;

	btConvexHullShape* tmp_shape = new btConvexHullShape(vertices, nb_vertices, 3*sizeof(float));
	btShapeHull* tmp_shape_hull = new btShapeHull(tmp_shape);
	tmp_shape_hull->buildHull(0);

	btAlignedObjectArray<btVector3> planeEquations;
	btAlignedObjectArray<btVector3> verticesCont;

	const btVector3* less_vertices = tmp_shape_hull->getVertexPointer();
	for(int i=0 ; i<tmp_shape_hull->numVertices() ; i++){
		verticesCont.push_back(less_vertices[i]);
	}

	btGeometryUtil::getPlaneEquationsFromVertices(verticesCont,planeEquations);

	btAlignedObjectArray<btVector3> shiftedPlaneEquations;
	for (int p=0;p<planeEquations.size();p++)
	{
			btVector3 plane = planeEquations[p];
			plane[3] += 3*collisionMargin;
			shiftedPlaneEquations.push_back(plane);
	}
	btAlignedObjectArray<btVector3> shiftedVertices;
	btGeometryUtil::getVerticesFromPlaneEquations(shiftedPlaneEquations,shiftedVertices);

	btConvexHullShape* old_shape = new btConvexHullShape(&(shiftedVertices[0].getX()),shiftedVertices.size());
	old_shape->setMargin(collisionMargin);
	btConvexPointCloudShape* shape;
	//shape->setMargin(0);

//	if(nb_vertices > 42) {
	{
		btShapeHull* shape_hull = new btShapeHull(old_shape);
		shape_hull->buildHull(0);//old_shape->getMargin());
		int nb_vertices = shape_hull->numVertices();
		btVector3* vertices = new btVector3[nb_vertices];
		memcpy(vertices,shape_hull->getVertexPointer(),nb_vertices*sizeof(btVector3));
		shape = new btConvexPointCloudShape(vertices,nb_vertices,btVector3(1,1,1));
		shape->setMargin(collisionMargin);
		delete shape_hull;
		delete old_shape;
	}

	m_convex_colliders.insert(name, shape);
}

btCollisionShape* PhysicsManager::getCollider(QString name)
{
	if(m_concave_colliders.contains(name)) {
		return m_concave_colliders.value(name, NULL);
	} else {
		btConvexPointCloudShape* shape = m_convex_colliders.value(name, NULL);
		if(shape != NULL) {
			shape = new btConvexPointCloudShape(*shape);
		}
		return shape;
	}
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
