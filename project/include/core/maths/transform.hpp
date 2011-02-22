// Transform.hpp
#ifndef TRANSFORM_HPP_
#define TRANSFORM_HPP_

#include "include/core/maths/plane.h"
#include "include/core/maths/transform.h"

// Constructeur
template <class T>
Transform<T>::Transform()
{
}

template<class T>
template <class T_scalar, class T_scalar2>
Transform<T>::Transform(const Matrix3<T_scalar> rotation, const Vector3<T_scalar2 >position)
{
	this->rotation=rotation;
	this->position=position;
}

template <class T>
template <class T_scalar>
Transform<T>::Transform(const Matrix4<T_scalar>& mat)
{
	*this = mat;
}

template <class T>
template <class T_scalar>
Transform<T>::Transform(const Transform<T_scalar>& ref)
{
	*this=ref;
}

template <class T>
Transform<T>::~Transform()
{
}

// Redefined operators
template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator=(const Transform<T_scalar>& ref)
{
	this->rotation=ref.rotation;
	this->position=ref.position;
	return *this;
}

template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator=(const Matrix4<T_scalar>& mat)
{
	this->rotation = Matrix3<T>(	Vector3<T>(mat[0],mat[1],mat[2]),
									Vector3<T>(mat[4],mat[5],mat[6]),
									Vector3<T>(mat[8],mat[9],mat[10]));
	this->position = Vector3<T>(mat[12], mat[13], mat[14]);
	return *this;
}

// translation along a vector
template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator+=(const Vector3<T_scalar>& ref)
{
	*this=*this+ref;
	return *this;
}

template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator-=(const Vector3<T_scalar>& ref)
{
	*this=*this-ref;
	return *this;
}

// dot product with an other transformation matrix
template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator*=(const Transform<T_scalar>& ref)
{
	*this=*this*ref;
	return *this;
}

template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator*=(const Matrix4<T_scalar>& ref)
{
	*this=*this*ref;
	return *this;
}

template <class T>
template <class T_scalar>
Transform<T>& Transform<T>::operator*=(const T_scalar ref[16])
{
	*this=*this*ref;
}

// translation along a vector
template <class T>
template <class T_scalar>
Transform<T> Transform<T>::operator+(const Vector3<T_scalar>& ref) const
{
	Transform<T> result=*this;
	result.position+=ref.position;
	return result;
}

template <class T>
template <class T_scalar>
Transform<T> Transform<T>::operator-(const Vector3<T_scalar>& ref) const
{
	Transform<T> result=*this;
	result.position-=ref.position;
	return result;
}

// product between transformation matrices
template <class T>
template <class T_scalar>
Transform<T> Transform<T>::operator*(const Transform<T_scalar>& ref) const
{

	Transform<T> result=*this;
	result.position+=this->rotation*ref.position;
	result.rotation*=ref.rotation;
	return result;

	//Matrix4<T> m_result = (Matrix4<T>)(*this) * (Matrix4<T>)ref;
	//return m_result;
}

template <class T>
template <class T_scalar>
Transform<T> Transform<T>::operator*(const Matrix4<T_scalar>& ref) const
{
	Transform<T> result=*this;
	result.rotation*=Matrix3<T>(	Vector3<T>(ref.values[0],ref.values[1],ref.values[2]),
									Vector3<T>(ref.values[4],ref.values[5],ref.values[6]),
									Vector3<T>(ref.values[8],ref.values[9],ref.values[10]));

	result.position+=Vector3<T>(ref.values[12],ref.values[13],ref.values[14]);
	return result;
}

template <class T>
template <class T_scalar>
Transform<T> Transform<T>::operator*(const T_scalar ref[16]) const
{
	Transform<T> result=*this;
	result.rotation*=Matrix3<T>(	Vector3<T>(ref[0],ref[1],ref[2]),
									Vector3<T>(ref[4],ref[5],ref[6]),
									Vector3<T>(ref[8],ref[9],ref[10]));

	result.position+=Vector3<T>(ref[12],ref[13],ref[14]);
	return result;
}

/*
// transformation of a vector
template <class T>
template <class T_scalar>
Vector3<T> Transform<T>::operator*(const Vector3<T_scalar>& ref) const
{
	return (rotation*ref)+position;
}
*/
//setters
template <class T>
template <class T_scalar>
void Transform<T>::setRotation(const Matrix3<T_scalar>& rotation)
{
	this->rotation=rotation;
}

template <class T>
template <class T_scalar, class T_scalar2>
void Transform<T>::rotate(Vector3<T_scalar> axis,T_scalar2 value)
{
	T_scalar c=fastCos(float(value)); T_scalar ic=1-c;
	T_scalar s=fastSin(float(value));
	axis.normalize();
	float xy=axis.x*axis.y;
	float yz=axis.y*axis.z;
	float xz=axis.x*axis.z;

	Matrix3<T_scalar> rot(	Vector3<T_scalar>(	(axis.x*axis.x*(ic)+c),	(xy*(ic)-axis.z*s),		(xz*(ic)+axis.y*s)		),
							Vector3<T_scalar>(	(xy*(ic)+axis.z*s),		(axis.y*axis.y*(ic)+c),	(yz*(ic)-axis.x*s)		),
							Vector3<T_scalar>(	(xz*(ic)-axis.y*s),		(yz*(ic)+axis.x*s),		(axis.z*axis.z*(ic)+c)	),false);

	rotation=rot*rotation;
}

template <class T>
template <class T_scalar>
void Transform<T>::lookAt(Vector3<T_scalar> target)
{
	// Calculus for the first angle
	Vector3<T> direction=(target-position);
	direction.normalize();
	rotation.setIdentity();
	Vector3<T> projected(direction.x,0.0,direction.z);

	// rotation along X
	if(target.y>=0)
		rotate(Vector3<T>(1.0,0.0,0.0),fastAcos(projected.norm()));
	else
		rotate(Vector3<T>(1.0,0.0,0.0),-fastAcos(projected.norm()));

	// Calculus for the second rotation
	projected.normalize();
	Vector3<T> axis=projected^(Vector3<T>(0.0,0.0,-1.0));

	// rotation along Y
	if(axis.y<=0)
	{
		if(axis.norm() && (Vector3<T>(0.0,0.0,-1.0)*projected)>=0)
			rotate(Vector3<T>(0.0,1.0,0.0),fastAsin(axis.norm()));
		else if(axis.norm() && (Vector3<T>(0.0,0.0,-1.0)*projected)<=0)
			rotate(Vector3<T>(0.0,1.0,0.0), 180.0-fastAsin(axis.norm()));
	}
	else
{
		if(axis.norm() && (Vector3<T>(0.0,0.0,-1.0)*projected)>=0)
			rotate(Vector3<T>(0.0,1.0,0.0),-fastAsin(axis.norm()));
		else if(axis.norm() && (Vector3<T>(0.0,0.0,-1.0)*projected)<=0)
			rotate(Vector3<T>(0.0,1.0,0.0),fastAsin(axis.norm()) - 180.0);
	}
	/*
	Vector3<T> axis=(Vector3<T>(1.0,0.0,0.0))^direction;
	rotation.setIdentity();
	if(axis.norm() && (Vector3<T>(1.0,0.0,0.0)*direction)>=0)
		rotate(axis,fastAsin(axis.norm()));
	else if(axis.norm() && (Vector3<T>(1.0,0.0,0.0)*direction)<=0)
		rotate(axis,180.0-fastAsin(axis.norm()));
	*/
}

template <class T>
template <class T_scalar>
void Transform<T>::move(const Vector3<T_scalar>& translation)
{
	position+=translation;
}

template <class T>
template <class T_scalar>
void Transform<T>::moveTo(const Vector3<T_scalar>& target)
{
	position=target;
}

#endif //TRANSFORM_HPP_
