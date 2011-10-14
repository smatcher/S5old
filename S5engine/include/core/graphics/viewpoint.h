#ifndef VIEWPOINT_H
#define VIEWPOINT_H

#include "core/maths/matrix4.h"

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

	virtual void setProjection(double aspect, int projection_nb) = 0;

	virtual void applyTransform(int projection_nb) = 0;
	virtual void applyOnlyRotation(int projection_nb)= 0;
};

#endif // VIEWPOINT_H
