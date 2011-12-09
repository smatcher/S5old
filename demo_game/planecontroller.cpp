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

	m_dragFactor = 0.06f;
	m_liftFactor = 100.0f;
	m_propulsionFactor = 1000.0f;
}

void PlaneController::update(double elapsed_time)
{
	btRigidBody* rgdBody = m_physicobject->getRigidBody();
	btVector3 _linearVelocity = rgdBody->getLinearVelocity();
	Matrix3f rotation = node()->getRotation(); // WARNING WONT WORK IF NODE HAS A ROTATED PARENT
	Vector3f linearVelocity(_linearVelocity.x(), _linearVelocity.y(), _linearVelocity.z());
	InputManager& manager  = INPUT_MANAGER;

	// Compute lift
	Vector3f forward = rotation*Vector3f(0,0,-1); // World forward vector
	Vector3f lift = rotation*Vector3f(0,1,0); // World lift support vector
	lift *= m_liftFactor * (forward.scaled(linearVelocity)).norm(); // Lift depends of the speed along the forward vector

	// Compute drag
	Vector3f drag = - m_dragFactor * linearVelocity.scaled(linearVelocity);

	// Compute propulsion
	Vector3f propulsion = m_propulsionFactor * forward;

	// Apply forces
	Vector3f globalForce = drag + lift + propulsion;
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
		globalTorque = rotation*globalTorque;
		rgdBody->applyTorque(btVector3(globalTorque.x, globalTorque.y, globalTorque.z));
	}
}
