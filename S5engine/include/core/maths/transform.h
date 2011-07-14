// Transform.h

#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "core/maths/vector3.h"
#include "core/maths/matrix3.h"
#include "core/maths/matrix4.h"
#include "core/maths/trigo.h"
#include <iostream>

#define TRANSFORM_OPENGL_SUPPORT

#ifdef TRANSFORM_OPENGL_SUPPORT
	#include <QtOpenGL>
#endif

enum TransformSpace
{
	LOCAL,
	PARENT,
	WORLD
};

template <class T>
class Transform
{
protected:
	Vector3<T> position;
	Vector3<T> scale_vector;
	Matrix3<T> rotation;

public:
	Transform();

	template <class T_scalar, class T_scalar2, class T_scalar3>
	Transform(const Matrix3<T_scalar> rotation, const Vector3<T_scalar2> position, Vector3<T_scalar3> scale);

	template <class T_scalar>
	Transform(const Matrix4<T_scalar>& mat);

	template <class T_scalar>
	Transform(const Transform<T_scalar>& ref);

	virtual ~Transform();

	template <class T_scalar>
	void copy(const Transform<T_scalar>& ref);

	// Redefined operators
	template <class T_scalar>
	Transform<T>& operator=(const Transform<T_scalar>& ref);

	template <class T_scalar>
	Transform<T>& operator=(const Matrix4<T_scalar>& mat);

	// translation along a vector
	template <class T_scalar>
	Transform<T>& operator+=(const Vector3<T_scalar>& ref);

	template <class T_scalar>
	Transform<T>& operator-=(const Vector3<T_scalar>& ref);

	// product with an other transformation matrix
	template <class T_scalar>
	Transform<T>& operator*=(const Transform<T_scalar>& ref);

	template <class T_scalar>
	Transform<T>& operator*=(const Matrix4<T_scalar>& ref);

	template <class T_scalar>
	Transform<T>& operator*=(const T_scalar ref[16]);

	// translation along a vector
	template <class T_scalar>
	Transform<T> operator+(const Vector3<T_scalar>& ref) const;

	template <class T_scalar>
	Transform<T> operator-(const Vector3<T_scalar>& ref) const;

	// product between transformation matrices
	template <class T_scalar>
	Transform<T> operator*(const Transform<T_scalar>& ref) const;

	template <class T_scalar>
	Transform<T> operator*(const Matrix4<T_scalar>& ref) const;

	template <class T_scalar>
	Transform<T> operator*(const T_scalar ref[16]) const;
	/*
	// transformation of a vector
	template <class T_scalar>
	Vector3<T> operator*(const Vector3<T_scalar>& ref) const;
	*/
	//Cast
	operator Matrix4<T> () {return Matrix4<T>(rotation, position, scale_vector);}
	operator const Matrix4<T> () const {return Matrix4<T>(rotation, position, scale_vector);}

	void invert()
	{
		Matrix4<T> mat = *this;
		mat.invert();
		*this = mat;
	//	rotation.invert(); position = Vector3<T>(-position.x,-position.y,-position.z);
	}

	// setters
	template <class T_scalar>
	void setRotation(const Matrix3<T_scalar>& rotation);

	template <class T_scalar, class T_scalar2>
	void rotate(Vector3<T_scalar> axis,T_scalar2 value);

	template <class T_scalar>
	void setScale(const Vector3<T_scalar>& scale);

	template <class T_scalar, class T_scalar2>
	void scale(Vector3<T_scalar> scale);

	template <class T_scalar>
	void lookAt(Vector3<T_scalar> target);

	template <class T_scalar>
	void move(const Vector3<T_scalar>& translation);

	template <class T_scalar>
	void moveTo(const Vector3<T_scalar>& target);

	// getters
	Transform<T> getInverse() const
	{
		Transform<T> t=(*this);
		t.invert();
		return t;
	}

	const Matrix3<T>& getRotation() const {return this->rotation;}
	const Vector3<T>& getPosition() const {return this->position;}
	const Vector3<T>& getScale()    const {return this->scale_vector;}

	// transformation of a vector
	template <class T_scalar>
	const Vector3<T_scalar> apply(const Vector3<T_scalar>& vec) const {return (rotation*vec)+position;}

	Vector3<T> toEuler() const;

#ifdef TRANSFORM_OPENGL_SUPPORT

	void glLoadf() const	{glLoadMatrixf(Matrix4f(rotation, position, scale_vector));}
	void glLoadd() const	{glLoadMatrixd(Matrix4d(rotation, position, scale_vector));}
	void glMultf() const	{glMultMatrixf(Matrix4f(rotation, position, scale_vector));}
	void glMultd() const	{glMultMatrixd(Matrix4d(rotation, position, scale_vector));}
	void glTranslatef() const	{::glTranslatef(position.x, position.y, position.z);}
	void glTranslated() const	{::glTranslated(position.x, position.y, position.z);}

#endif
};

// Iostream printing
template <class T>
inline std::ostream& operator<<(std::ostream& os, const Transform<T>& t)
{
	os << "rotation : " << t.getRotation() << " position : " << t.getPosition() << " scale : " << t.getScale();
	return os;
}

typedef Transform<double>			Transformd;
typedef Transform<float>			Transformf;
typedef Transform<unsigned int>		Transformui;
typedef Transform<int>				Transformi;
typedef Transform<short>			Transforms;
typedef Transform<unsigned char>	Transformub;

#include "core/maths/transform.hpp"

#endif // TRANSFORM_H
