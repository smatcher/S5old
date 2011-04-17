#ifndef DUMMYCONTROLABLE_H
#define DUMMYCONTROLABLE_H

#include "core/properties/iupdatable.h"

class DummyControlable : public IUpdatable
{
public:
	DummyControlable();
	void update(double elapsed_time);

private:
	int m_control_forward;
	int m_control_backward;
};

#endif // DUMMYCONTROLABLE_H
