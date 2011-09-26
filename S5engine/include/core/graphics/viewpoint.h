#ifndef VIEWPOINT_H
#define VIEWPOINT_H

class Viewpoint {
public:
	Viewpoint() {}
	virtual ~Viewpoint() {}

	virtual int getNbProjections() = 0;

	virtual const Matrix4d& getProjection(double aspect, int projection_nb) = 0 ;
	virtual void setProjection(double aspect, int projection_nb) = 0;

	virtual void applyTransform(int projection_nb) = 0;
	virtual void applyOnlyRotation(int projection_nb)= 0;
};

#endif // VIEWPOINT_H
