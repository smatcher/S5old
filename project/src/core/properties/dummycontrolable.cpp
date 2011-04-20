#include "core/properties/dummycontrolable.h"
#include "core/inputs/inputmanager.h"
#include "core/scenegraph/node.h"

#include <QDebug>

DummyControlable::DummyControlable()
{
	InputManager& manager  = INPUT_MANAGER;
	m_control_forward = manager.getControlId("avance");
	m_control_backward = manager.getControlId("recule");
	m_control_left = manager.getControlId("gauche");
	m_control_right = manager.getControlId("droite");
}

void DummyControlable::update(double elapsed_time)
{
	InputManager& manager  = INPUT_MANAGER;

	if(manager.getButtonStatus(m_control_forward) == InputManager::Held)
	{
		Node* parentNode = this->node();
		Vector3d move = parentNode->getRotation() * Vector3d(0.0, 0.0, -elapsed_time);
		parentNode->move(move);
	}
	if(manager.getButtonStatus(m_control_backward) == InputManager::Held)
	{
		Node* parentNode = this->node();
		Vector3d move = parentNode->getRotation() * Vector3d(0.0, 0.0, elapsed_time);
		parentNode->move(move);
	}
	if(manager.getButtonStatus(m_control_left) == InputManager::Held)
	{
		Node* parentNode = this->node();
		parentNode->rotate(Vector3d(0.0, 1.0, 0.0), 130 * elapsed_time);
	}
	if(manager.getButtonStatus(m_control_right) == InputManager::Held)
	{
		Node* parentNode = this->node();
		parentNode->rotate(Vector3d(0.0, 1.0, 0.0), - 130 * elapsed_time);
	}
}
