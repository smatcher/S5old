#include "planecontroller.h"
#include "core/inputs/inputmanager.h"
#include "core/scenegraph/node.h"

PlaneController::PlaneController(PhysicObject* physicobject)
{
	m_physicobject = physicobject;
	InputManager& manager  = INPUT_MANAGER;
	m_control_forward = manager.getControlId("avance");
	m_control_backward = manager.getControlId("recule");
	m_control_left = manager.getControlId("gauche");
	m_control_right = manager.getControlId("droite");
}

void PlaneController::update(double elapsed_time)
{
	btRigidBody* rgdBody = m_physicobject->getRigidBody();
	InputManager& manager  = INPUT_MANAGER;
	Vector3f globalForce = node()->apply(Vector3f(0,1000,-2000));
	rgdBody->applyCentralForce(btVector3(globalForce.x, globalForce.y, globalForce.z));
	Vector3f globalTorque;
	bool apply = FALSE;
	if(manager.getButtonStatus(m_control_forward) == InputManager::Held) {
		globalTorque.x -= 900; apply = TRUE;
	}
	if(manager.getButtonStatus(m_control_backward) == InputManager::Held) {
		globalTorque.x += 900; apply = TRUE;
	}
	if(manager.getButtonStatus(m_control_left) == InputManager::Held) {
		globalTorque.z += 1100; apply = TRUE;
	}
	if(manager.getButtonStatus(m_control_right) == InputManager::Held) {
		globalTorque.z -= 1100; apply = TRUE;
	}
	if(apply) {
		globalTorque = node()->apply(globalTorque);
		rgdBody->applyTorque(btVector3(globalTorque.x, globalTorque.y, globalTorque.z));
	}
}
