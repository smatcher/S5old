#include "dummyjumper.h"
#include <core/scenegraph/node.h>
#include <core/maths/trigo.h>

DummyJumper::DummyJumper(double speed, double amplitude) : IUpdatable(QString("DummyJumper"))
{
	m_speed = speed;
	m_amplitude = amplitude;
	m_phase = 90;
}

void DummyJumper::update(double elapsed_time)
{
	m_phase += 360 * m_speed * elapsed_time;
	if(m_phase > 360) {
		m_phase -= 360;
	}
	node()->move(m_amplitude*m_speed*elapsed_time*fastCos(m_phase)*Vector3f(0,1,0)); // Instable, integrale powered (yeah :D)
}

