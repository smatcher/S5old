#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/physicsmanager.h"
#include "btBulletDynamicsCommon.h"
#include "BulletCollision/CollisionShapes/btShapeHull.h"

class PhysicObject : public IProperty, public Managee<PhysicsManager>, public btMotionState
{
public:

	enum ColliderShape
	{
		BOX,
		SPHERE,
		MESH
	};

	struct Properties
	{
		float mass;
		float restitution;
		float linDamping;
		float angDamping;
		ColliderShape shape;
		QString mesh_name;
		bool is_kinematic;

		Properties() :
			mass(0.0),
			restitution(0.0),
			linDamping(0.0),
			angDamping(0.0),
			shape(BOX),
			is_kinematic(false)
		{}
	};

	PhysicObject(PhysicObject::Properties properties, const QString& name = "PhysicObject");
	virtual ~PhysicObject();

	btRigidBody* getRigidBody();

	virtual void getWorldTransform(btTransform &worldTrans) const;
	virtual void setWorldTransform(const btTransform &worldTrans);

private:
	btRigidBody*          m_body;
	btCollisionShape*     m_shape;
	btShapeHull*          m_cached_shape;

	PhysicObject::Properties m_properties;

	/// Debug
	virtual void drawDebug(const GLWidget*, const RenderManager::DebugGizmosFilter& filter) const;

	virtual void onLinked(PropertySet *);
	virtual void onUnlinked(PropertySet *);
};

#endif // PHYSICSOBJECT_H
