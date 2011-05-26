// frustum.cpp

#include "core/maths/frustum.h"
#include "core/maths/constants.h"
/*
#include <GL/glew.h>
#include <GL/glfw.h>
#include "../opengl/RAII.h"
*/
#include <iostream>

using namespace std;

Frustum::Frustum()
{
}

Frustum::~Frustum()
{
}

void Frustum::setFromMatrix(const Matrix4f& matrix)
{
	near_plane.setPlaneEquation(	matrix[3] + matrix[2], matrix[7] + matrix[6], matrix[11] + matrix[10],
									matrix[15] + matrix[14]);

	far_plane.setPlaneEquation(	matrix[3] - matrix[2], matrix[7] - matrix[6], matrix[11] - matrix[10],
									matrix[15] - matrix[14]);

	left_plane.setPlaneEquation(	matrix[3] + matrix[0], matrix[7] + matrix[4], matrix[11] + matrix[8],
									matrix[15] + matrix[12]);

	right_plane.setPlaneEquation(	matrix[3] - matrix[0], matrix[7] - matrix[4], matrix[11] - matrix[8],
									matrix[15] - matrix[12]);

	bottom_plane.setPlaneEquation(	matrix[3] + matrix[1], matrix[7] + matrix[5], matrix[11] + matrix[9],
									matrix[15] + matrix[13]);

	top_plane.setPlaneEquation(	matrix[3] - matrix[1], matrix[7] - matrix[5], matrix[11] - matrix[9],
								matrix[15] - matrix[13]);
}

bool Frustum::isInside(const Vector3f& point) const
{
	if(left_plane.getDistanceToPlane(point)-EPSILON < 0.0)
		return false;

	if(right_plane.getDistanceToPlane(point)-EPSILON < 0.0)
		return false;

	if(bottom_plane.getDistanceToPlane(point)-EPSILON < 0.0)
		return false;

	if(top_plane.getDistanceToPlane(point)-EPSILON < 0.0)
		return false;

	if(near_plane.getDistanceToPlane(point)-EPSILON < 0.0)
		return false;

	if(far_plane.getDistanceToPlane(point)-EPSILON < 0.0)
		return false;

	return true;
}

bool Frustum::isInside(const Vector3f& point, float margin) const
{
	if(left_plane.getDistanceToPlane(point)+margin-EPSILON < 0.0)
		return false;

	if(right_plane.getDistanceToPlane(point)+margin-EPSILON < 0.0)
		return false;

	if(bottom_plane.getDistanceToPlane(point)+margin-EPSILON < 0.0)
		return false;

	if(top_plane.getDistanceToPlane(point)+margin-EPSILON < 0.0)
		return false;

	if(near_plane.getDistanceToPlane(point)+margin-EPSILON < 0.0)
		return false;

	if(far_plane.getDistanceToPlane(point)+margin-EPSILON < 0.0)
		return false;

	return true;
}

bool Frustum::isInside(const Vector3d& point) const
{
	return isInside((Vector3f)point);
}

bool Frustum::isInside(const Vector3d& point, double margin) const
{
	return isInside((Vector3f)point,(float)margin);
}

bool Frustum::isInside(const BoundingVolume& volume) const
{
	return volume.isInsideFrustum(*this);
}

/*
void Frustum::debugDraw(const Vector4ub& color, const Vector3ub& lines_color)
{
	Disable<GL_CULL_FACE> cull_face_state;
	Disable<GL_TEXTURE_2D> texture_2D_state;
	Disable<GL_LIGHTING> lighting_state;

	// Intersection points
	Vector3f near_left_bottom, near_right_bottom;
	Vector3f near_left_top, near_right_top;

	Vector3f far_left_bottom, far_right_bottom;
	Vector3f far_left_top, far_right_top;

	// Compute the intersection points
	near_left_bottom = near_plane.getIntersection(left_plane, bottom_plane);
	near_right_bottom = near_plane.getIntersection(right_plane, bottom_plane);

	near_left_top = near_plane.getIntersection(left_plane, top_plane);
	near_right_top = near_plane.getIntersection(right_plane, top_plane);

	far_left_bottom = far_plane.getIntersection(left_plane, bottom_plane);
	far_right_bottom = far_plane.getIntersection(right_plane, bottom_plane);

	far_left_top = far_plane.getIntersection(left_plane, top_plane);
	far_right_top = far_plane.getIntersection(right_plane, top_plane);

	// Draw the lines
	{
		Disable<GL_BLEND> blend_state;
		glColor3ubv(lines_color);
		glLineWidth(3.0);

		// near :
		glBegin(GL_LINE_LOOP);
		glVertex3fv(near_left_bottom);
		glVertex3fv(near_right_bottom);
		glVertex3fv(near_right_top);
		glVertex3fv(near_left_top);
		glEnd();

		// far :
		glBegin(GL_LINE_LOOP);
		glVertex3fv(far_left_bottom);
		glVertex3fv(far_right_bottom);
		glVertex3fv(far_right_top);
		glVertex3fv(far_left_top);
		glEnd();

		// left :
		glBegin(GL_LINE_LOOP);
		glVertex3fv(near_left_bottom);
		glVertex3fv(far_left_bottom);
		glVertex3fv(far_left_top);
		glVertex3fv(near_left_top);
		glEnd();

		// right :
		glBegin(GL_LINE_LOOP);
		glVertex3fv(near_right_bottom);
		glVertex3fv(far_right_bottom);
		glVertex3fv(far_right_top);
		glVertex3fv(near_right_top);
		glEnd();

		// top :
		glBegin(GL_LINE_LOOP);
		glVertex3fv(near_left_top);
		glVertex3fv(near_right_top);
		glVertex3fv(far_right_top);
		glVertex3fv(far_left_top);
		glEnd();

		// bottom:
		glBegin(GL_LINE_LOOP);
		glVertex3fv(near_left_bottom);
		glVertex3fv(near_right_bottom);
		glVertex3fv(far_right_bottom);
		glVertex3fv(far_left_bottom);
		glEnd();

		glLineWidth(1.0);
	}

	// Draw the faces
	{
		glColor4ubv(color);
		glBegin(GL_QUADS);

		// near :
		glVertex3fv(near_left_bottom);
		glVertex3fv(near_right_bottom);
		glVertex3fv(near_right_top);
		glVertex3fv(near_left_top);

		// far :
		glVertex3fv(far_left_bottom);
		glVertex3fv(far_right_bottom);
		glVertex3fv(far_right_top);
		glVertex3fv(far_left_top);

		// left :
		glVertex3fv(near_left_bottom);
		glVertex3fv(far_left_bottom);
		glVertex3fv(far_left_top);
		glVertex3fv(near_left_top);

		// right :
		glVertex3fv(near_right_bottom);
		glVertex3fv(far_right_bottom);
		glVertex3fv(far_right_top);
		glVertex3fv(near_right_top);

		// top :
		glVertex3fv(near_left_top);
		glVertex3fv(near_right_top);
		glVertex3fv(far_right_top);
		glVertex3fv(far_left_top);

		// bottom:
		glVertex3fv(near_left_bottom);
		glVertex3fv(near_right_bottom);
		glVertex3fv(far_right_bottom);
		glVertex3fv(far_left_bottom);

		glEnd();
	}
}
*/
