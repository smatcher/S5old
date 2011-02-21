// Frustum.h

#ifndef FRUSTUM_H
#define FRUSTUM_H

#include "include/core/maths/plane.h"
#include "include/core/maths/boundingvolume.h"
#include "include/core/maths/matrix4.h"

class Frustum
{
protected:
	Planef near_plane;
	Planef far_plane;
	Planef left_plane;
	Planef right_plane;
	Planef top_plane;
	Planef bottom_plane;

public:
	Frustum();
	virtual ~Frustum();

	void setFromMatrix(const Matrix4f& matrix);
/*
	void debugDraw(	const Vector4ub& color = Vector4ub(0x11, 0xEE, 0xFF, 0x77),
					const Vector3ub& lines_color = Vector3ub(0xFF, 0x00, 0x00));
*/
	bool isInside(const Vector3f& point) const;
	bool isInside(const Vector3f& point, float margin) const;
	bool isInside(const Vector3d& point) const;
	bool isInside(const Vector3d& point, double margin) const;
	bool isInside(const BoundingVolume& volume) const;

	const Planef& getNearPlane() const		{return near_plane;}
	const Planef& getFarPlane() const		{return far_plane;}
	const Planef& getLeftPlane() const		{return left_plane;}
	const Planef& getRightPlane() const		{return right_plane;}
	const Planef& getTopPlane() const		{return top_plane;}
	const Planef& getBottomPlane() const	{return bottom_plane;}
};

#endif // FRUSTUM_H
