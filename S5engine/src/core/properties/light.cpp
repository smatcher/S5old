#include "core/properties/light.h"
#include "core/scenegraph/node.h"

const double posx[] = {
	 0, 0,-1, 0,
	 0,-1, 0, 0,
	-1, 0, 0, 0,
	 0, 0, 0, 1
};

const double posy[] = {
	 1, 0, 0, 0,
	 0, 0,-1, 0,
	 0, 1, 0, 0,
	 0, 0, 0, 1
};

const double posz[] = {
	 1, 0, 0, 0,
	 0,-1, 0, 0,
	 0, 0,-1, 0,
	 0, 0, 0, 1
};

const double negx[] = {
	 0, 0, 1, 0,
	 0,-1, 0, 0,
	 1, 0, 0, 0,
	 0, 0, 0, 1
};

const double negy[] = {
	 1, 0, 0, 0,
	 0, 0, 1, 0,
	 0,-1, 0, 0,
	 0, 0, 0, 1
};

const double negz[] = {
	-1, 0, 0, 0,
	 0,-1, 0, 0,
	 0, 0, 1, 0,
	 0, 0, 0, 1
};

Light::Light() : IProperty("Light")
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

void Light::drawDebug(const GLWidget* widget) const
{
		widget->qglColor(Qt::yellow);
		glBegin(GL_LINE_LOOP);
		for(int i=0 ; i<360 ; i+=10) {
			float sin = fastSin(i)/10;
			float cos = fastCos(i)/10;
			glVertex3d(0,sin,cos);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(int i=0 ; i<360 ; i+=10) {
			float sin = fastSin(i)/10;
			float cos = fastCos(i)/10;
			glVertex3d(sin,0,cos);
		}
		glEnd();
		glBegin(GL_LINE_LOOP);
		for(int i=0 ; i<360 ; i+=10) {
			float sin = fastSin(i)/10;
			float cos = fastCos(i)/10;
			glVertex3d(sin,cos,0);
		}
		glEnd();
}

int Light::getNbProjections()
{
	return 6;
}

Viewpoint::Style Light::getStyle()
{
	return PROXY_CUBEMAP;
}

void Light::setProjection(double aspect, int projection_nb)
{
	Matrix4d mat;
	const float h = 1.0f/tan(90*M_PI/360);
	const float znear = 0.1;
	const float zfar = 100;
	float neg_depth = znear-zfar;

	mat[0] = h;
	mat[1] = 0;
	mat[2] = 0;
	mat[3] = 0;

	mat[4] = 0;
	mat[5] = h;
	mat[6] = 0;
	mat[7] = 0;

	mat[8] = 0;
	mat[9] = 0;
	mat[10] = (zfar + znear)/neg_depth;
	mat[11] = -1;

	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 2.0f*(znear*zfar)/neg_depth;
	mat[15] = 0;

	switch(projection_nb) {
		case 0:
			mat *= Matrix4d(posx);
			break;
		case 1:
			mat *= Matrix4d(negx);
			break;
		case 2:
			mat *= Matrix4d(posy);
			break;
		case 3:
			mat *= Matrix4d(negy);
			break;
		case 4:
			mat *= Matrix4d(posz);
			break;
		case 5:
			mat *= Matrix4d(negz);
			break;
	}
	glLoadMatrixd(mat);
}

void Light::applyTransform(int projection_nb)
{
	//node()->getGlobalTransform().getInverse().glMultd();

	Transformf trans(node()->getGlobalTransform());
	float transx = -trans.getPosition().x;
	float transy = -trans.getPosition().y;
	float transz = -trans.getPosition().z;
	glTranslatef(transx, transy, transz);
}

void Light::applyOnlyRotation(int projection_nb)
{
	/*
	Transformf trans(node()->getGlobalTransform());
	Matrix3d rotation = trans.getRotation();
	Transformf transform(rotation.getInverse(),Vector3f(),Vector3f(1,1,1));
	transform.glMultd();
	*/
}
