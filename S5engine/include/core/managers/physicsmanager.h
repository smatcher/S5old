#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class PhysicObject;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btCollisionDispatcher;
class btCollisionConfiguration;
class btSequentialImpulseConstraintSolver;
class GLDebugDrawer;

class PhysicsManager : public Manager<PhysicObject>
{
	friend class Singleton<PhysicsManager>;

protected:
	PhysicsManager();

public:
	virtual ~PhysicsManager();

	void update(double elapsed);
	void onManageeLinked(PhysicObject *managee);

	void debugDraw();

private:
	btDiscreteDynamicsWorld*             m_discreteDynamicsWorld;
	btBroadphaseInterface*               m_broadphaseInterface;
	btCollisionDispatcher*               m_collisionDispatcher;
	btCollisionConfiguration*            m_collisionConfiguration;
	btSequentialImpulseConstraintSolver* m_sequentialImpulseConstraintSolver;

	GLDebugDrawer* m_debug;
};

typedef Singleton<PhysicsManager> SingletonPhysicsManager;
#define PHYSICS_MANAGER SingletonPhysicsManager::getInstance()

#endif // PHYSICSMANAGER_H
