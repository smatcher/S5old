// Plane.h

#ifndef PLANE_H
#define PLANE_H

#include "include/core/maths/vector3.h"
#include "include/core/maths/constants.h"
#include <iostream>

template <class T>
class Plane
{
public:
	// Equation du plan : ax + by + cz + d = 0
	Vector3<T> normal;	// Vecteur normal au plan : N(a, b, c)
	T d;	// Distance entre le plan et (0, 0, 0)

	enum PointPosition
	{
		POINT_FRONT,
		POINT_ON,
		POINT_BACK
	};

public:
	// Constructeurs
	Plane()
	{
		// Par défaut : on plan en 0 qui pointe vers le haut (Y > 0)
		normal = Vector3<T>(0.0, 1.0, 0.0);
		d = 0.0;
	}

	Plane(const Vector3<T>& normal, const T d)
	{
		this->normal = normal;
		this->d = d;
	}

	Plane(const Plane& ref)
	{
		*this = ref;
	}

	// Destructeur
	~Plane()
	{
	}

	// Opérateur d'affectation
	Plane& operator=(const Plane& ref)
	{
		this->normal = ref.normal;
		this->d = ref.d;
	}

	Vector3f getNormal(){return normal;}
	T getDistance(){return d;}

	void setPlaneEquation(T a, T b, T c, T d)
	{
		setPlaneEquation(Vector3<T>(a, b, c), d);
	}

	void setPlaneEquation(const Vector3<T>& normal, T d)
	{
		T norm = normal.norm();
		this->normal = normal / norm;
		this->d = d / norm;
	}

	// Détermination du plan à partir de 3 points
	void setFromPoints(const Vector3<T>& p0, const Vector3<T>& p1, const Vector3<T>& p2)
	{
		normal = (p1-p0) * (p2-p0);

		normal.normalize();

		calculateD(p0);
	}

	// Calcul de la distance entre 0 et le plan à partir d'un point appartenant au plan
	void calculateD(const Vector3<T>& point_on_plane)
	{
		this->d = - normal * point_on_plane;
	}

	T getDistanceToPlane(const Vector3<T>& point) const
	{
		return point * normal + d;
	}

	PointPosition classifyPoint(const Vector3<T>& point) const
	{
		T dist_to_plane = getDistanceToPlane(point);
		if(dist_to_plane < -EPSILON)
			return POINT_BACK;
		else if(dist_to_plane > EPSILON)
			return POINT_FRONT;
		else
			return POINT_ON;
	};

	// Normalisation
	void normalize()
	{
		normal.normalize();
	}

	// Calcul du point d'intersection de 3 plans.
	// *intersect indique si les 3 plans s'intersectent ou pas.
	// An explanation here : http://geometryalgorithms.com/Archive/algorithm_0104/algorithm_0104B.htm
	Vector3<T> getIntersection(const Plane<T>& p2, const Plane<T>& p3, bool* intersect=NULL)
	{
		Vector3<T> result;

		T denominator = normal * (p2.normal ^ p3.normal);

		if(denominator == 0.0)
		{
			if(intersect != NULL)
				*intersect = false;
			return Vector3<T>(0.0);
		}
		else
		{
			if(intersect != NULL)
				*intersect = true;
		}

		Vector3<T> temp1, temp2, temp3;

		temp1 = d * (p2.normal ^ p3.normal);
		temp2 = p2.d * (p3.normal ^ normal);
		temp3 = p3.d * (normal ^ p2.normal);

		result = (temp1 + temp2 + temp3) / (-denominator);

		return result;
	}
};

typedef Plane<float> Planef;
typedef Plane<double> Planed;

#endif // PLANE_H
