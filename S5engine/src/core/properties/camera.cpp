#include "core/properties/camera.h"
#include <core/graphics/framebufferobject.h>
#include <core/graphics/rendertarget.h>
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

	m_render_texture = NULL;
}

Camera::~Camera()
{
}


void Camera::createTarget(int height, int width)
{
	if(node()) {
		m_render_texture = new RenderTexture("RTT_"+getName(), height, width, GL_RGBA, GL_UNSIGNED_BYTE);
		FrameBufferObject* fbo = new FrameBufferObject(height, width, false, true);
		fbo->attachTexture(m_render_texture, FrameBufferObject::COLOR_ATTACHMENT);
		RenderTarget* target = new RenderTarget(this, fbo, height, width, false);
		RENDER_MANAGER.addRenderTarget(target);
	} else {
		logError("Can't create RTT from unliked camera");
	}
}

Texture Camera::getTargetTexture()
{
	return Texture(*m_render_texture);
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

void Camera::setProjection(double aspect, int projection_nb)
{
//	gluPerspective(m_yfov,aspect,m_znear,m_zfar);
	glLoadMatrixd(getProjection(aspect, projection_nb));
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

