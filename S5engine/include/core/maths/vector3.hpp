// Vector3.hpp
// Vecteur en 3 dimensions
// 2008 par Funto
// Version 1.0

#include <math.h>

template <class T>
Vector3<T>::Vector3()
: x(coords[0]), y(coords[1]), z(coords[2]), r(coords[0]), g(coords[1]), b(coords[2])
{
	x = T(0);
	y = T(0);
	z = T(0);
}

template <class T>
Vector3<T>::Vector3(const T& x, const T& y, const T& z)
: x(coords[0]), y(coords[1]), z(coords[2]), r(coords[0]), g(coords[1]), b(coords[2])
{
	this->x = x;
	this->y = y;
	this->z = z;
}

template <class T>
template <class T_scalar>
Vector3<T>::Vector3(const T_scalar& s)
: x(coords[0]), y(coords[1]), z(coords[2]), r(coords[0]), g(coords[1]), b(coords[2])
{
	this->x = s;
	this->y = s;
	this->z = s;
}

template <class T>
Vector3<T>::Vector3(const Vector3<T>& ref)
: x(coords[0]), y(coords[1]), z(coords[2]), r(coords[0]), g(coords[1]), b(coords[2])
{
	*this = ref;
}

template <class T>
template <class T_2>
Vector3<T>::Vector3(const Vector3<T_2>& ref)
: x(coords[0]), y(coords[1]), z(coords[2]), r(coords[0]), g(coords[1]), b(coords[2])
{
	*this = ref;
}

template <class T>
Vector3<T>::~Vector3()
{
}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator=(const Vector3<T_2>& ref)
{
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	return *this;
}

// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector3<T>& Vector3<T>::operator=(const Vector3<T>& ref)
{
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	return *this;
}

// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator=(const T_scalar& s)
{
	this->x = s;
	this->y = s;
	this->z = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator+=(const Vector3<T_2>& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator-=(const Vector3<T_2>& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	return *this;
}

// Affectation-produit vectoriel
template <class T>
template <class T_2>
Vector3<T>& Vector3<T>::operator^=(const Vector3<T_2>& v)
{
	T old_x = this->x;
	T old_y = this->y;

	this->x = this->y * v.z - this->z * v.y;
	this->y = - (old_x * v.z - this->z * v.x);
	this->z = old_x * v.y - old_y * v.x;

	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator*=(const T_scalar& s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector3<T>& Vector3<T>::operator/=(const T_scalar& s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	return *this;
}

// Addition
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator+(const Vector3<T_2>& v) const
{
	return Vector3<T>(this->x + v.x, this->y + v.y, this->z + v.z);
}

// Soustraction
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator-(const Vector3<T_2>& v) const
{
	return Vector3<T>(this->x - v.x, this->y - v.y, this->z - v.z);
}

// Produit vectoriel
template <class T>
template <class T_2>
Vector3<T> Vector3<T>::operator^(const Vector3<T_2>& v) const
{
	Vector3<T> resultat;
	resultat.x = this->y * v.z - this->z * v.y;
	resultat.y = - (this->x * v.z - this->z * v.x);
	resultat.z = this->x * v.y - this->y * v.x;
	return resultat;
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector3<T> Vector3<T>::operator*(const T_scalar& s) const
{
	return Vector3<T>(this->x * s, this->y * s, this->z * s);
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Vector3<T> Vector3<T>::operator/(const T_scalar& s) const
{
	return Vector3<T>(this->x / s, this->y / s, this->z / s);
}

// Produit scalaire
template <class T>
T Vector3<T>::operator*(const Vector3<T>& v) const
{
	return this->x * v.x + this->y * v.y + this->z * v.z;
}

template <class T>
template <class T_scalar>
Vector3<T> Vector3<T>::scaled(const Vector3<T_scalar>& scale) const
{
	return Vector3<T>(this->x*scale.x, this->y*scale.y, this->z*scale.z);
}

// Normalisation + renvoi de la valeur de la norme
template <class T>
double Vector3<T>::normalize()
{
	double norm = sqrtf(x*x + y*y + z*z);
	x = T(double(x) / norm);
	y = T(double(y) / norm);
	z = T(double(z) / norm);

	return norm;
}

// Calcul de la norme
template <class T>
double Vector3<T>::norm() const
{
	return sqrtf(x*x + y*y + z*z);
}

// Calcul de la norme au carré (plus rapide)
template <class T>
T Vector3<T>::squaredNorm() const
{
	return x*x + y*y + z*z;
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector3<T> operator*(const T_scalar& s, const Vector3<T>& v)
{
	return Vector3<T>(v.x * s, v.y * s, v.z * s);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector3<T> operator/(const T_scalar& s, const Vector3<T>& v)
{
	return Vector3<T>(v.x / s, v.y / s, v.z / s);
}

// Interpolation
template<class T>
Vector3<T> Vector3<T>::lerp(double factor, const Vector3<T>& vec1, const Vector3<T>& vec2)
{
	return (1-factor)*vec1 + factor*vec2;
}


