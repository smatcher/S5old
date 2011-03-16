#include "core/properties/camera.h"
#include <QtOpenGL>

Camera::Camera(double yfov, double znear, double zfar) : IProperty("Camera")
{
	m_yfov = yfov;
	m_znear = znear;
	m_zfar = zfar;
	m_needComputation = true;
	m_lastAspect = 0;
}

Camera::~Camera()
{
}

const Matrix4d& Camera::getProjection(double aspect)
{
	if(m_lastAspect != aspect || m_needComputation)
	{
		aspect = m_lastAspect;
		computeProjection();
	}
	return m_projection;
}

void Camera::setProjection(double aspect)
{
	gluPerspective(m_yfov,aspect,m_znear,m_zfar);
}

void Camera::setParameters(double yfov, double znear, double zfar)
{
	m_yfov = yfov;
	m_znear = znear;
	m_zfar = zfar;
	m_needComputation = true;
}

void Camera::computeProjection()
{
	double xymax = m_znear * tan(m_yfov * M_PI / 360);
	double ymin = -xymax;
	double xmin = -xymax;

	double width = xymax - xmin;
	double height = xymax - ymin;

	double depth = m_zfar - m_znear;
	double q = -(m_zfar + m_znear) / depth;
	double qn = -2 * (m_zfar * m_znear) / depth;

	double w = 2 * m_znear / width;
	w = w / m_lastAspect;
	double h = 2 * m_znear / height;

	m_projection[0]  = w;
	m_projection[1]  = 0;
	m_projection[2]  = 0;
	m_projection[3]  = 0;

	m_projection[4]  = 0;
	m_projection[5]  = h;
	m_projection[6]  = 0;
	m_projection[7]  = 0;

	m_projection[8]  = 0;
	m_projection[9]  = 0;
	m_projection[10] = q;
	m_projection[11] = -1;

	m_projection[12] = 0;
	m_projection[13] = 0;
	m_projection[14] = qn;
	m_projection[15] = 0;
}
