#include "core/properties/physicobject.h"

#include "btBulletDynamicsCommon.h"
#include "core/scenegraph/node.h"
#include "core/maths/vector3.h"

#include "core/log/log.h"

PhysicObject::PhysicObject(PhysicObject::Properties properties, const QString& name) : IProperty(name), Managee<PhysicsManager>()
{
	m_properties = properties;
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
	btScalar mass = m_properties.mass;
	btScalar restitution = m_properties.restitution;
	btVector3 localInertia(0,0,0);
	btTransform myTransform;
	/*
	myTransform.setIdentity();
	myTransform.setOrigin( btVector3(0,5,0) );
	*/
	btScalar mat[16];
	Matrix4d nodeMat = node()->getGlobalTransform(false);
	for(int i=0 ;i<16 ; i++) {
		mat[i] = nodeMat[i];
	}
	myTransform.setFromOpenGLMatrix(mat);
	Vector3f nodeScale = node()->getScale();
	btVector3 size(nodeScale.x,nodeScale.y,nodeScale.z);
	size *= 0.5;

	switch(m_properties.shape) {
		case SPHERE:
			m_shape = new btSphereShape(nodeScale.x/2);
			break;
		case MESH:
		default :
			m_shape = new btBoxShape(size);
	}
	if(mass)
	{
		m_shape->calculateLocalInertia( mass, localInertia );
	}

	btRigidBody::btRigidBodyConstructionInfo myBoxInfo(mass, this, m_shape, localInertia);
	m_body = new btRigidBody(myBoxInfo);
	m_body->setRestitution(restitution);

	if(mass == 0.0 && m_properties.is_kinematic)
	{
		m_body->setCollisionFlags( m_body->getCollisionFlags() |
		btCollisionObject::CF_KINEMATIC_OBJECT);
		m_body->setActivationState(DISABLE_DEACTIVATION);
	}

	PHYSICS_MANAGER.onManageeLinked(this);
}

void PhysicObject::onUnlinked(PropertySet *)
{
}

void PhysicObject::getWorldTransform(btTransform &worldTrans) const
{
	btScalar mat[16];
	Matrix4d nodeMat = node()->getGlobalTransform(false);
	for(int i=0 ;i<16 ; i++) {
		mat[i] = nodeMat[i];
	}
	worldTrans.setFromOpenGLMatrix(mat);
}

void PhysicObject::setWorldTransform(const btTransform &worldTrans)
{
	/*
	btScalar matrix[16];
	worldTrans.getOpenGLMatrix(matrix);
	node()->moveTo(Vector3f(matrix[12],matrix[13],matrix[14]));
	*/
	btScalar matrix[16];
	worldTrans.getOpenGLMatrix(matrix);
	node()->setGlobalTransform(Transformf(Matrix4f(matrix)));
}
