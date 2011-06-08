#ifndef PHYSICSOBJECT_H
#define PHYSICSOBJECT_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/physicsmanager.h"

class btRigidBody;
class btDefaultMotionState;
class btCollisionShape;

class PhysicObject : public IProperty, public Managee<PhysicsManager>
{
public:
	PhysicObject(float mass, const QString& name = "PhysicObject");
	virtual ~PhysicObject();

	btRigidBody* getRigidBody();
	void onPhysicUpdate();

private:
	btRigidBody*          m_body;
	btDefaultMotionState* m_motion;
	btCollisionShape*     m_shape;

	float m_mass;

	/// Debug
	virtual void drawDebug(const GLWidget*) const;

	virtual void onLinked(PropertySet *);
	virtual void onUnlinked(PropertySet *);
};

#endif // PHYSICSOBJECT_H
