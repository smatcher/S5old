#include "core/properties/camera.h"
#include "core/framework/glwidget.h"
#include <QtOpenGL>

Camera::Camera(double yfov, double znear, double zfar) : IProperty("Camera"), Managee()
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
        m_lastAspect = aspect;
		computeProjection();
	}
	return m_projection;
}

void Camera::setProjection(double aspect)
{
//	gluPerspective(m_yfov,aspect,m_znear,m_zfar);
    glLoadMatrixd(getProjection(aspect));
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

void Camera::drawDebug(const GLWidget* widget) const
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
