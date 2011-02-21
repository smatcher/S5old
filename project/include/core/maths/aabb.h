// AABB.h
// Axis Aligned Bounding Box
// I wanted to call it BoundingBox for short so just complain to the bearded guy if you too don't like the name ;)

#ifndef AABB_H
#define AABB_H

#include "include/core/maths/boundingvolume.h"

class AABB : public BoundingVolume
{
protected:
	Vector3f half_dimensions;	// Coordinates of the up right near corner relative to the center of the box

	Vector3f corners[8];	// Used for optimizing the frustum test

private:
	void updateCorners();

public:
	AABB(const Vector3f& center=Vector3f(0.0, 0.0, 0.0), const Vector3f& half_dimensions=Vector3f(1.0, 1.0, 1.0));
	AABB(const AABB& ref);
	virtual ~AABB();

	AABB& operator=(const AABB& ref);

	// Overloaded, to keep the corners' positions updated
	virtual void moveTo(const Vector3f& center);

	virtual bool pointIsInside(const Vector3f& point, const Vector3f& object_position=Vector3f(0.0, 0.0, 0.0)) const;
	virtual bool pointIsInside(const Vector3d& point, const Vector3d& object_position=Vector3d(0.0, 0.0, 0.0)) const;

	// Check if the volume intersects or is inside a given frustum.
	// object_position : position of the bounded object
	virtual bool isInsideFrustum(	const Frustum& frustum,
									const Vector3f& object_position = Vector3f(0.0, 0.0, 0.0)) const;

	void setHalfDimensions(const Vector3f& half_dimensions);
	const Vector3f& getHalfDimensions() const {return this->half_dimensions;}

	const Vector3f* getCorners() const {return corners;}

	virtual Type getType() {return BOX;}
};

#endif // AABB_H
