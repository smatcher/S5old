#ifndef PHYSICSMANAGER_H
#define PHYSICSMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

#include <QMap>
#include <QString>

class PhysicObject;
class btDiscreteDynamicsWorld;
class btBroadphaseInterface;
class btCollisionShape;
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

	void buildConvexCollider(QString name, float* vertices, int nb_vertices);
	//void buildConcaveCollider(QString name, float* vertices);

	btCollisionShape* getCollider(QString name);

private:
	btDiscreteDynamicsWorld*             m_discreteDynamicsWorld;
	btBroadphaseInterface*               m_broadphaseInterface;
	btCollisionDispatcher*               m_collisionDispatcher;
	btCollisionConfiguration*            m_collisionConfiguration;
	btSequentialImpulseConstraintSolver* m_sequentialImpulseConstraintSolver;

	GLDebugDrawer* m_debug;

	QMap<QString,btCollisionShape*> m_colliders;
};

typedef Singleton<PhysicsManager> SingletonPhysicsManager;
#define PHYSICS_MANAGER SingletonPhysicsManager::getInstance()

#endif // PHYSICSMANAGER_H
