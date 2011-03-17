#ifndef CAMERA_H
#define CAMERA_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/cameramanager.h"
#include "core/maths/matrix4.h"

class Camera : public IProperty, public Managee<CameraManager>
{
public :
	/// Constructror
	Camera(double yfov, double znear, double zfar);
	/// Destructor
	virtual ~Camera();

	const Matrix4d& getProjection(double aspect);
	void setProjection(double aspect);

	void setParameters(double yfov, double znear, double zfar);

private :
	bool m_needComputation;
	double m_yfov;
	double m_znear;
	double m_zfar;
	double m_lastAspect;

	Matrix4d m_projection;

    void computeProjection();

    /// Debug
    virtual void drawDebug(const GLWidget*) const;
};

#endif // CAMERA_H
