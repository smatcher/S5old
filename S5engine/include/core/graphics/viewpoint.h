#ifndef VIEWPOINT_H
#define VIEWPOINT_H

#include "core/maths/vector3.h"
#include "core/maths/matrix4.h"
#include "core/maths/frustum.h"

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
	virtual Frustum getFrustum() = 0;

	virtual void setProjection(double aspect, double scale, int projection_nb) = 0;

	virtual void applyTransform(int projection_nb) = 0;
	virtual void applyOnlyRotation(int projection_nb)= 0;
};

#endif // VIEWPOINT_H
