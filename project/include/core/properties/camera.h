#ifndef CAMERA_H
#define CAMERA_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/cameramanager.h"
#include "core/maths/matrix4.h"

class CameraRadioButton;
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

	void setRadioButton(CameraRadioButton* radio);

private :
	bool m_needComputation;
	double m_yfov;
	double m_znear;
	double m_zfar;
	double m_lastAspect;

	CameraRadioButton* m_radiobutton;

	Matrix4d m_projection;

    void computeProjection();

    /// Debug
	virtual void drawDebug(const GLWidget*) const;
	virtual void onLinked(PropertySet *);
	virtual void onUnlinked(PropertySet *);
};

#endif // CAMERA_H
