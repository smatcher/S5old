#include "core/properties/dummycontrolable.h"
#include "core/inputs/inputmanager.h"
#include "core/scenegraph/node.h"

DummyControlable::DummyControlable()
{
	InputManager& manager  = INPUT_MANAGER::getInstance();
	m_control_forward = manager.getControlId("avance");
	m_control_backward = manager.getControlId("recule");
}

void DummyControlable::update(double elapsed_time)
{
	InputManager& manager  = INPUT_MANAGER::getInstance();

	if(manager.getButtonStatus(m_control_forward) == InputManager::Pressed)
	{
		Node* parentNode = this->node();
		parentNode->move(Vector3d(0.0, 0.0, elapsed_time));
	}
	if(manager.getButtonStatus(m_control_backward) == InputManager::Pressed)
	{
		Node* parentNode = this->node();
		parentNode->move(Vector3d(0.0, 0.0, -elapsed_time));
	}
}
