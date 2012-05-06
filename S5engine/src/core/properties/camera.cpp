#include "core/properties/camera.h"
#include <core/graphics/framebufferobject.h>
#include <core/graphics/rendertarget.h>
#include <core/graphics/rt2d.h>
#include "core/framework/glwidget.h"
#include "core/utils/customevents.h"
#include <QtOpenGL>

#ifdef WITH_TOOLS
	#include "tools/mvc/cameramodel.h"
#endif

Camera::Camera(double yfov, double znear, double zfar) : IProperty("Camera"), Managee<CameraManager>()
{
	m_yfov = yfov;
	m_znear = znear;
	m_zfar = zfar;
	m_needComputation = true;
	m_lastAspect = 0;

	m_render_texture = 0;
}

Camera::~Camera()
{
}


void Camera::createTarget(int height, int width)
{
	if(node()) {
		m_render_texture = new RenderTexture2D("RTT_"+node()->getName(), height, width, IRD::Texture::TF_RGBA8);
		FrameBufferObject* fbo = new FrameBufferObject(height, width, false, true);
		RenderTarget* target = new RenderTarget(this, fbo, m_render_texture, IRD::FrameBuffer::COLOR_ATTACHMENT_0, false, false);
		RENDER_MANAGER.addRenderTarget(target);
	} else {
		logError("Can't create RTT from unliked camera");
	}
}

Texture Camera::getTargetTexture()
{
	return Texture(*m_render_texture);
}

Matrix4d Camera::getViewProjection(double aspect, int projection_nb)
{
	if(m_lastAspect != aspect || m_needComputation)
	{
		m_lastAspect = aspect;
		computeProjection();
	}

	Matrix4d view = node()->getGlobalTransform().getInverse();
	debug("MATRIX_STACK", "get viewprojection : view" << QMatrix4x4(view.values));
	debug("MATRIX_STACK", "get viewprojection : viewprojection" << QMatrix4x4((m_projection*view).values));
	return m_projection*view;
}

const Matrix4d& Camera::getProjection(double aspect, int projection_nb)
{
	if(m_lastAspect != aspect || m_needComputation)
	{
		m_lastAspect = aspect;
		computeProjection();
	}

	return m_projection;
}

void Camera::setProjection(double aspect, double scale, int projection_nb)
{
//	gluPerspective(m_yfov,aspect,m_znear,m_zfar);
	glLoadMatrixd(getProjection(aspect, projection_nb));
	glScaled(scale, scale, 1);
}

void Camera::setParameters(double yfov, double znear, double zfar)
{
	m_yfov = yfov;
	m_znear = znear;
	m_zfar = zfar;
	m_needComputation = true;
}

void computeProjectionFunction(Matrix4d& mat, double yfov, double znear, double zfar, double aspect)
{
	const float h = 1.0f/tan(yfov*M_PI/360);
	float neg_depth = znear-zfar;

	mat[0] = h / aspect;
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
}

void Camera::computeProjection()
{
	computeProjectionFunction(m_projection, m_yfov, m_znear, m_zfar, m_lastAspect);
}

void Camera::drawDebug(const GLWidget* widget, const RenderManager::DebugGizmosFilter& filter) const
{
	if(filter.draw_cameras)
	{
		glPushMatrix();

			Matrix4d mat;
			computeProjectionFunction(mat, m_yfov, m_znear, m_zfar, 1);
			mat.invert();
			glMultMatrixd(mat.values);

			widget->qglColor(Qt::gray);
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

				glVertex3d(-1,-1, 1);
				glVertex3d( 1,-1, 1);

				glVertex3d(-1,-1, 1);
				glVertex3d(-1, 1, 1);

				glVertex3d( 1, 1, 1);
				glVertex3d(-1, 1, 1);

				glVertex3d( 1, 1, 1);
				glVertex3d( 1,-1, 1);

				glVertex3d( 1, 1, 1);
				glVertex3d( 1, 1,-1);
			glEnd();
		glPopMatrix();
	}
}

void Camera::applyTransform(int projection_nb)
{
	node()->getGlobalTransform().getInverse().glMultd();
}

void Camera::applyOnlyRotation(int projection_nb)
{
	Transformf trans(node()->getGlobalTransform());
	Matrix3d rotation = trans.getRotation();
	Transformf transform(rotation.getInverse(),Vector3f(),Vector3f(1,1,1));
	transform.glMultd();
}

Vector3f Camera::getWorldPosition()
{
	if(node())
	{
		Matrix4f globalTrans = node()->getGlobalTransform();
		return Vector3f(globalTrans[12], globalTrans[13], globalTrans[14]);
	}

	return Vector3f();
}

Frustum Camera::getFrustum(int projection_nb)
{
	return Frustum();
}

IRD::Viewport Camera::getViewport(int projection_nb)
{
	IRD::Viewport vp;
	vp.x = 0;
	vp.y = 0;
	vp.width = 1;
	vp.height = 1;
	vp.relative = true;
	return vp;
}

#ifdef WITH_TOOLS

void Camera::onLinked(PropertySet *)
{
	QCoreApplication::postEvent(CAMERA_MANAGER.getDebugModel(), new UPDATED_EVENT());
}

void Camera::onUnlinked(PropertySet *)
{
	QCoreApplication::postEvent(CAMERA_MANAGER.getDebugModel(), new UPDATED_EVENT());
}

#endif

