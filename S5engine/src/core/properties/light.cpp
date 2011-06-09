#include "core/properties/light.h"
#include "core/scenegraph/node.h"

Light::Light()
{
}

void Light::sendParameters(int lightid)
{
	glEnable(GL_LIGHT0 + lightid);
	glEnable(GL_MULTISAMPLE);
	Matrix4f trans = node()->getGlobalTransform();
	Vector3<GLfloat> pos(trans[12],trans[13],trans[14]);
	GLfloat lightPosition[4] = { 0.0, 0.0, 0.0, 1.0 };
	for(int i=0 ; i<3 ; i++) {
		lightPosition[i] = pos[i];
	}
	glLightfv(GL_LIGHT0 + lightid, GL_POSITION, lightPosition);
	// Create light components for GL_LIGHT0
	float ambientLight0[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	float diffuseLight0[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	float specularLight0[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	// Assign created components to GL_LIGHT0
	glLightfv(GL_LIGHT0 + lightid, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0 + lightid, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0 + lightid, GL_SPECULAR, specularLight0);
}
