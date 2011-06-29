#ifndef DUMMYUPDATABLE_H
#define DUMMYUPDATABLE_H

#include "core/properties/iupdatable.h"

class DummyUpdatable : public IUpdatable
{
public:
	DummyUpdatable(double speed = 0.1);
	void update(double elapsed_time);

private:
	double angle;
	double speed;

};

#endif // DUMMYUPDATABLE_H
