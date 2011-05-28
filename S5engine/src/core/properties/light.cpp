#include "core/properties/light.h"
#include "core/scenegraph/node.h"

Light::Light()
{
}

void Light::sendParameters(int lightid)
{
	glEnable(GL_LIGHT0 + lightid);
	glEnable(GL_MULTISAMPLE);
	Vector3<GLfloat> pos = node()->globalTransform().getPosition();
	GLfloat lightPosition[4] = { 0.0, 0.0, 0.0, 1.0 };
	for(int i=0 ; i<3 ; i++) {
		lightPosition[i] = pos[i];
	}
	glLightfv(GL_LIGHT0 + lightid, GL_POSITION, lightPosition);
}
