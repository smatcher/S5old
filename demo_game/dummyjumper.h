#ifndef DUMMY_JUMPER_H
#define DUMMY_JUMPER_H

#include <core/properties/iupdatable.h>

class DummyJumper : public IUpdatable
{
public :
	DummyJumper(double speed, double amplitude);
	void update(double elapsed_time);

private:
	double m_speed;
	double m_amplitude;
	double m_phase;
};

#endif //DUMMY_JUMPER_H
