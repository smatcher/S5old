#include "core/properties/physicobject.h"

#include "btBulletDynamicsCommon.h"
#include "core/scenegraph/node.h"
#include "core/maths/vector3.h"

#include "core/log/log.h"

PhysicObject::PhysicObject(PhysicObject::Properties properties, const QString& name) : IProperty(name), Managee<PhysicsManager>()
{
	m_properties = properties;
	m_cached_shape = NULL;
	m_shape = NULL;
}

PhysicObject::~PhysicObject()
{
	if(m_cached_shape != NULL)
		delete m_cached_shape;
	if(m_shape != NULL)
		delete m_shape;
}

btRigidBody* PhysicObject::getRigidBody()
{
	return m_body;
}

void PhysicObject::drawDebug(const GLWidget*, const RenderManager::DebugGizmosFilter& filter) const
{
	if(filter.draw_colliders)
	{
		if(m_cached_shape != NULL) {
			const unsigned int* indices = m_cached_shape->getIndexPointer();
			const btVector3* vertices = m_cached_shape->getVertexPointer();
			unsigned int num_indices = m_cached_shape->numIndices();
			glColor3f(1,1,1);
			for(int i=0 ; i+2< num_indices ; i+=3) {
				glBegin(GL_LINE_STRIP);
					glVertex3fv(vertices[indices[i]]);
					glVertex3fv(vertices[indices[i+1]]);
					glVertex3fv(vertices[indices[i+2]]);
				glEnd();
			}
		}
	}
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
	btConvexShape* shape;
	btCompoundShape* cmpd_shape;
	btVector3 pos(0,0,0);
	btScalar radi = 0.5;

	switch(m_properties.shape) {
		case SPHERE:
			m_shape = new btMultiSphereShape(&pos,&radi,1);
			m_shape->setLocalScaling(size);
			break;
		case MESH:
			shape = dynamic_cast<btConvexShape*>(PHYSICS_MANAGER.getCollider(m_properties.mesh_name));
			if(shape != NULL) {
				m_cached_shape = new btShapeHull(shape);
				m_cached_shape->buildHull(shape->getMargin());
	//			cmpd_shape = new btCompoundShape();
	//			cmpd_shape->addChildShape(btTransform::getIdentity(),shape);
	//			m_shape = cmpd_shape;
				m_shape = shape;
				m_shape->setLocalScaling(size);
			}
			break;
		default :
			m_shape = new btBoxShape(size * 0.5);
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
