#include "core/properties/light.h"
#include "core/scenegraph/node.h"
#include "core/log/log.h"
#include "core/resources/managers.h"
#include <core/graphics/rtarray.h>
#include <core/graphics/rt2d.h>
#include <GL/glu.h>

/*
#define OMNIDEPTH_RESOLUTION 512
#define SUNDEPTH_RESOLUTION 2048
*/
#define OMNIDEPTH_RESOLUTION 128
#define SUNDEPTH_RESOLUTION 512

#ifdef _WIN32
	#define GL_MULTISAMPLE 0x809D
#endif

const double posx[] = {
	 1, 0, 0, 0,
	 0,-1, 0, 0,
	 0, 0,-1, 0,
	 0, 0, 0, 1
};

const double posy[] = {
	 1, 0, 0, 0,
	 0, 0,-1, 0,
	 0, 1, 0, 0,
	 0, 0, 0, 1
};

const double posz[] = {
	 0, 0,-1, 0,
	 0,-1, 0, 0,
	-1, 0, 0, 0,
	 0, 0, 0, 1
};

const double negx[] = {
	-1, 0, 0, 0,
	 0,-1, 0, 0,
	 0, 0, 1, 0,
	 0, 0, 0, 1
};

const double negy[] = {
	 1, 0, 0, 0,
	 0, 0, 1, 0,
	 0,-1, 0, 0,
	 0, 0, 0, 1
};

const double negz[] = {
	 0, 0, 1, 0,
	 0,-1, 0, 0,
	 1, 0, 0, 0,
	 0, 0, 0, 1
};

Light::Light(bool casts_shadows) : IProperty("Light"), m_casts_shadows(casts_shadows), m_shadowmap(0)
{
	m_constant_attenuation = 1.0f;
	m_linear_attenuation = 0.0f;
	m_quadratic_attenuation = 0.25f;

	m_diffuse_color = Vector4f(1.0,1.0,1.0,1.0);
	m_specular_color = Vector4f(1.0,1.0,1.0,1.0);

	setSpotCutoff(30.f);

	m_type = OMNI;
}

void Light::sendParameters(int lightid)
{
	debugGL("before sending light parameters for light" << lightid);

	glEnable(GL_LIGHT0 + lightid);
	glEnable(GL_MULTISAMPLE);
	Matrix4f trans = node()->getGlobalTransform();
	Vector3<GLfloat> pos(trans[12],trans[13],trans[14]);
	GLfloat lightPosition[4] = { 0.0, 0.0, 0.0, 1.0 };

	if(m_type == SUN)
		lightPosition[3] = 0.0;

	for(int i=0 ; i<3 ; i++) {
		lightPosition[i] = pos[i];
	}
	glLightfv(GL_LIGHT0 + lightid, GL_POSITION, lightPosition);
	glLightf(GL_LIGHT0 + lightid, GL_CONSTANT_ATTENUATION, m_constant_attenuation);
	glLightf(GL_LIGHT0 + lightid, GL_LINEAR_ATTENUATION, m_linear_attenuation);
	glLightf(GL_LIGHT0 + lightid, GL_QUADRATIC_ATTENUATION, m_quadratic_attenuation);
	glLightfv(GL_LIGHT0 + lightid, GL_DIFFUSE, m_diffuse_color.coords);
	glLightfv(GL_LIGHT0 + lightid, GL_SPECULAR, m_specular_color.coords);

	if(m_type == SPOT || m_type == SUN) {
		GLfloat lightDirection[4] = { trans[0], trans[1], trans[2], 1.0 };
		glLightf(GL_LIGHT0 + lightid, GL_SPOT_CUTOFF,m_cos_spot_cutoff);
		glLightfv(GL_LIGHT0 + lightid, GL_SPOT_DIRECTION,lightDirection);
	}

	debugGL("while sending light parameters for light" << lightid);
}

void Light::drawDebug(const GLWidget* widget, const RenderManager::DebugGizmosFilter& filter) const
{
	if(filter.draw_lights)
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


		if(m_type == SPOT || m_type == SUN)
		{
			glPushMatrix();

			Matrix4d mat;
			computeLightFrustum(mat);
			mat.invert();
			glMultMatrixd(mat.values);

			glBegin(GL_LINES);
				glVertex3d(-1,-1,-1);
				glVertex3d( 1,-1,-1);

				glVertex3d(-1,-1,-1);
				glVertex3d(-1, 1,-1);

				glVertex3d(-1,-1,-1);
				glVertex3d(-1,-1, 1);

				glVertex3d( 1,-1,-1);
				glVertex3d( 1, 1,-1);

				glVertex3d( 1,-1,-1);
				glVertex3d( 1,-1, 1);

				glVertex3d(-1, 1,-1);
				glVertex3d( 1, 1,-1);

				glVertex3d(-1, 1,-1);
				glVertex3d(-1, 1, 1);

		widget->qglColor(Qt::red);
				glVertex3d(-1,-1, 1);
				glVertex3d( 1,-1, 1);

				glVertex3d(-1,-1, 1);
				glVertex3d(-1, 1, 1);

				glVertex3d( 1, 1, 1);
				glVertex3d(-1, 1, 1);

				glVertex3d( 1, 1, 1);
				glVertex3d( 1,-1, 1);

		widget->qglColor(Qt::yellow);
				glVertex3d( 1, 1, 1);
				glVertex3d( 1, 1,-1);
			glEnd();

			glPopMatrix();
		}
	}
}

int Light::getNbProjections()
{
	int ret = 0;

	switch(m_type)
	{
	case SPOT:
		ret = 1;
		break;
	case OMNI:
		ret = 6;
		break;
	case SUN:
		ret = 1;
		break;
	}

	return ret;
}

Viewpoint::Style Light::getStyle()
{
	if(m_type == OMNI)
		return PROXY_CUBEMAP;
	else
		return MONO;
}

void Light::setProjection(double aspect, double scale, int projection_nb)
{
	Matrix4d mat;
	computeLightFrustum(mat);

	if(m_type == OMNI)
	{
		switch(projection_nb) {
			case 0:
				mat *= Matrix4d(negx);
				break;
			case 1:
				mat *= Matrix4d(posx);
				break;
			case 2:
				mat *= Matrix4d(negy);
				break;
			case 3:
				mat *= Matrix4d(posy);
				break;
			case 4:
				mat *= Matrix4d(negz);
				break;
			case 5:
				mat *= Matrix4d(posz);
				break;
		}
	}
	glLoadMatrixd(mat);
}

void Light::applyTransform(int projection_nb)
{
	if(m_type == OMNI)
	{
		Transformf trans(node()->getGlobalTransform());
		float transx = -trans.getPosition().x;
		float transy = -trans.getPosition().y;
		float transz = -trans.getPosition().z;
		glTranslatef(transx, transy, transz);
	}
	else
	{
		node()->getGlobalTransform().getInverse().glMultd();
	}
}

void Light::applyOnlyRotation(int projection_nb)
{
	if(m_type == OMNI)
		return;

	Transformf trans(node()->getGlobalTransform());
	Matrix3d rotation = trans.getRotation();
	Transformf transform(rotation.getInverse(),Vector3f(),Vector3f(1,1,1));
	transform.glMultd();
}

bool Light::castsShadows()
{
	return m_casts_shadows;
}

RenderTexture* Light::getRenderTexture()
{
	if(m_shadowmap == 0) {
		int i=0;
		if(m_type == OMNI)
		{
			while(TEXTURE_MANAGER.get("Omni_Shadowmap"+QString().setNum(i)).isValid()) i++;
			m_shadowmap = new RenderTexture2D("Omni_Shadowmap"+QString().setNum(i), 2*OMNIDEPTH_RESOLUTION, 3*OMNIDEPTH_RESOLUTION, IRD::Texture::TF_DEPTH);
			//m_shadowmap = new RenderTextureArray("Omni_Shadowmap"+QString().setNum(i), OMNIDEPTH_RESOLUTION, OMNIDEPTH_RESOLUTION, 6, IRD::Texture::TF_DEPTH);
		}
		else if(m_type == SPOT)
		{
			while(TEXTURE_MANAGER.get("Spot_Shadowmap"+QString().setNum(i)).isValid()) i++;
			m_shadowmap = new RenderTexture2D("Spot_Shadowmap"+QString().setNum(i), OMNIDEPTH_RESOLUTION, OMNIDEPTH_RESOLUTION, IRD::Texture::TF_DEPTH);
		}
		else if(m_type == SUN)
		{
			while(TEXTURE_MANAGER.get("Sun_Shadowmap"+QString().setNum(i)).isValid()) i++;
			m_shadowmap = new RenderTexture2D("Sun_Shadowmap"+QString().setNum(i), SUNDEPTH_RESOLUTION, SUNDEPTH_RESOLUTION, IRD::Texture::TF_DEPTH);
		}
	}

	return m_shadowmap;
}

void Light::setDiffuseColor(Vector4f color)
{
	m_diffuse_color = color;
}

void Light::setSpecularColor(Vector4f color)
{
	m_specular_color = color;
}

void Light::setAttenuation(float constant, float linear, float quadratic)
{
	m_constant_attenuation = constant;
	m_linear_attenuation = linear;
	m_quadratic_attenuation = quadratic;
}

void Light::getAttenuation(float& constant, float& linear, float& quadratic)
{
	constant = m_constant_attenuation;
	linear = m_linear_attenuation;
	quadratic = m_quadratic_attenuation;
}

void Light::setType(Type type)
{
	if(m_type != type && m_shadowmap != 0)
	{
		m_shadowmap = 0; //MEMORY LEAK
	}

	m_type = type;
}

Light::Type Light::getType()
{
	return m_type;
}

void Light::setSpotCutoff(float degAngle)
{
	m_cos_spot_cutoff = cos(degAngle * M_PI/180);
	m_spot_cutoff = degAngle;
}

void Light::computeLightFrustum(Matrix4d& mat) const
{
	if(m_type == SUN)
	{
		/*
		const float h = 1.0f/tan(90*M_PI/360);
		const float znear = 0.5;
		const float zfar = 512;
		float neg_depth = znear-zfar;
		*/
		const float w = 0.001;
		const float h = 0.001;
		const float d = 0.0026;

		mat[0] = 0;
		mat[1] = 0;
		mat[2] = -d;
		mat[3] = 0;

		mat[4] = 0;
		mat[5] = h;
		mat[6] = 0;
		mat[7] = 0;

		mat[8] = w;
		mat[9] = 0;
		mat[10] = 0;
		mat[11] = 0;

		mat[12] = 0;
		mat[13] = 0;
		mat[14] = -1;
		mat[15] = 1;
	}
	else
	{
		float angle = 90;

		if(m_type == SPOT)
		{
			angle = 2*m_spot_cutoff;
		}

		const float h = 1.0f/tan(angle*M_PI/360);
		const float znear = 0.5;
		const float zfar = 512;
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

		if(m_type == SPOT)
		{
			mat *= Matrix4d(negz);
		}
	}
}


Vector3f Light::getWorldPosition()
{
	if(node())
	{
		Matrix4f globalTrans = node()->getGlobalTransform();
		return Vector3f(globalTrans[12], globalTrans[13], globalTrans[14]);
	}

	return Vector3f();
}

Frustum Light::getFrustum(int projection_nb)
{
	return Frustum();
}

IRD::Viewport Light::getViewport(int projection_nb)
{
	IRD::Viewport vp;

	if(m_type == OMNI)
	{
		int row = projection_nb/3;
		int col = projection_nb%3;
		vp.x = (float)col/3.0f;
		vp.y = (float)row/2.0f;
		vp.width = 1.0f/3.0f;
		vp.height = 1.0/2.0f;
		vp.relative = true;
	}
	else
	{
		vp.x = 0;
		vp.y = 0;
		vp.width = 1;
		vp.height = 1;
		vp.relative = true;
	}

	return vp;
}

bool Light::mustClearTMP(int projection_nb)
{
	return projection_nb == 0;
}
