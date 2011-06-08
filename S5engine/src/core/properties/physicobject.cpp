#include "core/properties/physicobject.h"

#include "btBulletDynamicsCommon.h"
#include "core/scenegraph/node.h"
#include "core/maths/vector3.h"

#include "core/log/log.h"

PhysicObject::PhysicObject(float mass, const QString& name) : IProperty(name), Managee<PhysicsManager>()
{
	m_mass = mass;
}

PhysicObject::~PhysicObject()
{
}

btRigidBody* PhysicObject::getRigidBody()
{
	return m_body;
}

void PhysicObject::drawDebug(const GLWidget*) const
{
}

void PhysicObject::onLinked(PropertySet*)
{
	btScalar mass = m_mass;
	btVector3 localInertia(0,0,0);
	btTransform myTransform;
	/*
	myTransform.setIdentity();
	myTransform.setOrigin( btVector3(0,5,0) );
	*/
	btScalar mat[16];
	Matrix4d nodeMat = node()->globalTransform();
	for(int i=0 ;i<16 ; i++) {
		mat[i] = nodeMat[i];
	}
	myTransform.setFromOpenGLMatrix(mat);
	Vector3d nodeScale = node()->getScale();
	btVector3 size(nodeScale.x,nodeScale.y,nodeScale.z);
	m_shape = new btBoxShape(size);
	if(mass)
		m_shape->calculateLocalInertia( mass, localInertia );
	m_motion = new btDefaultMotionState(myTransform);
	btRigidBody::btRigidBodyConstructionInfo myBoxInfo(mass, m_motion, m_shape, localInertia);
	m_body = new btRigidBody(myBoxInfo);

	PHYSICS_MANAGER.onManageeLinked(this);
}

void PhysicObject::onUnlinked(PropertySet *)
{
}

void PhysicObject::onPhysicUpdate()
{
	btScalar matrix[16];
	m_motion->m_graphicsWorldTrans.getOpenGLMatrix(matrix);
	node()->moveTo(Vector3d(matrix[12],matrix[13],matrix[14]));
	debug("PHYSIC",matrix[12] << matrix[13] << matrix[14]);
}

