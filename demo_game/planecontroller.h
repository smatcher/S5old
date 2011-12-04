#ifndef PLANECONTROLLER_H
#define PLANECONTROLLER_H

#include "core/properties/iupdatable.h"
#include "core/properties/physicobject.h"

class PlaneController : public IUpdatable
{
public:
	PlaneController(PhysicObject* physicobject);
	virtual void update(double elapsed_time);

private:
	PhysicObject* m_physicobject;
	int m_control_forward;
	int m_control_backward;
	int m_control_left;
	int m_control_right;
};

#endif // PLANECONTROLLER_H
