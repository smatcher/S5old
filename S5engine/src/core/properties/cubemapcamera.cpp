#include "core/properties/cubemapcamera.h"
#include "core/scenegraph/node.h"
#include <core/graphics/framebufferobject.h>
#include <core/graphics/rendertarget.h>
#include "core/graphics/rtcubemap.h"
#include "core/managers/rendermanager.h"

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

CubemapCamera::CubemapCamera(double znear, double zfar) : IProperty("CubemapCamera"),
	m_znear(znear), m_zfar(zfar)
{
}

CubemapCamera::~CubemapCamera()
{
}

void CubemapCamera::setProjection(double aspect, double scale,int projection_nb)
{
	Matrix4d mat;
	const float h = 1.0f/tan(90*M_PI/360);
	float neg_depth = m_znear-m_zfar;

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
	mat[10] = (m_zfar + m_znear)/neg_depth;
	mat[11] = -1;

	mat[12] = 0;
	mat[13] = 0;
	mat[14] = 2.0f*(m_znear*m_zfar)/neg_depth;
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

void CubemapCamera::applyTransform(int projection_nb)
{
	//node()->getGlobalTransform().getInverse().glMultd();

	Transformf trans(node()->getGlobalTransform());
	float transx = -trans.getPosition().x;
	float transy = -trans.getPosition().y;
	float transz = -trans.getPosition().z;
	glTranslatef(transx, transy, transz);
}

void CubemapCamera::applyOnlyRotation(int projection_nb)
{
	/*
	Transformf trans(node()->getGlobalTransform());
	Matrix3d rotation = trans.getRotation();
	Transformf transform(rotation.getInverse(),Vector3f(),Vector3f(1,1,1));
	transform.glMultd();
	*/
}

void CubemapCamera::createTarget(int height, int width)
{
	if(node()) {
		m_render_texture = new RenderTextureCubemap("RTT_"+node()->getName(), height, width, GL_RGBA, GL_UNSIGNED_BYTE);
		FrameBufferObject* fbo = new FrameBufferObject(height, width, false, true);
		RenderTarget* target = new RenderTarget(this, fbo, m_render_texture, FrameBufferObject::COLOR_ATTACHMENT, false, false);
		RENDER_MANAGER.addRenderTarget(target);
	} else {
		logError("Can't create RTT from unliked camera");
	}
}

Texture CubemapCamera::getTargetTexture()
{
	return Texture(*m_render_texture);
}
