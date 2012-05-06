#ifndef VIEWPOINT_H
#define VIEWPOINT_H

#include "core/maths/vector3.h"
#include "core/maths/matrix4.h"
#include "core/maths/frustum.h"
#include "core/abstraction/interface/irenderdevice.h"

class Viewpoint {
public:

	enum Style {
		MONO,
		CUBEMAP,
		PROXY_CUBEMAP
	};

	Viewpoint() {}
	virtual ~Viewpoint() {}

	virtual int getNbProjections() = 0;
	virtual Style getStyle() = 0;

	virtual Vector3f getWorldPosition() = 0;
	virtual Frustum getFrustum(int projection_nb) = 0;
	virtual IRD::Viewport getViewport(int projection_nb) = 0;
	virtual bool mustClearTMP(int projection_nb) {return true;}

	virtual Matrix4d getViewProjection(double aspect, int projection_nb) = 0;
	virtual const Matrix4d& getProjection(double aspect, int projection_nb) = 0;
	virtual void setProjection(double aspect, double scale, int projection_nb) = 0;

	virtual void applyTransform(int projection_nb) = 0;
	virtual void applyOnlyRotation(int projection_nb)= 0;
};

#endif // VIEWPOINT_H
