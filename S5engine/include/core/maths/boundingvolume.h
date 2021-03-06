// BoundingVolume.h

#ifndef BOUNDING_VOLUME_H
#define BOUNDING_VOLUME_H

#include "core/maths/vector3.h"

class Frustum;

class BoundingVolume
{
protected:
	Vector3f center;	// Position of the bounding volume, relative to the position of the bounded object.

public:
	enum Type
	{
		SPHERE,
		BOX
	};

public:
	BoundingVolume(const Vector3f& center=Vector3f(0.0, 0.0, 0.0))
	{
		moveTo(center);
	}

	BoundingVolume(const BoundingVolume& ref)
	{
		*this = ref;
	}

	virtual ~BoundingVolume()
	{
	}

	BoundingVolume& operator=(const BoundingVolume& ref)
	{
		this->center = ref.center;
		return *this;
	}

	void setCenter(const Vector3f& center)
	{
		this->center = center;
	}

	const Vector3f& getCenter() const
	{
		return center;
	}

	virtual void moveTo(const Vector3f& center)
	{
		this->center=center;
	}

	virtual bool pointIsInside(const Vector3f& point, const Vector3f& object_position=Vector3f(0.0, 0.0, 0.0)) const = 0;
	virtual bool pointIsInside(const Vector3d& point, const Vector3d& object_position=Vector3d(0.0, 0.0, 0.0)) const = 0;

	// Check if the volume intersects or is inside a given frustum.
	// object_position : position of the bounded object
	virtual bool isInsideFrustum(	const Frustum& frustum,
									const Vector3f& object_position = Vector3f(0.0, 0.0, 0.0)) const = 0;

	virtual Type getType() = 0;
};

#endif // BOUNDING_VOLUME_H
