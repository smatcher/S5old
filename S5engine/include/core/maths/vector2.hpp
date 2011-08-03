// Vector2.hpp
// Vecteur en 2 dimensions
// 2008 par Funto
// Version 1.0

#include <math.h>

template <class T>
Vector2<T>::Vector2()
: x(coords[0]), y(coords[1])
{
	x = T(0);
	y = T(0);
}

template <class T>
Vector2<T>::Vector2(const T& x, const T& y)
: x(coords[0]), y(coords[1])
{
	this->x = x;
	this->y = y;
}

template <class T>
template <class T_scalar>
Vector2<T>::Vector2(const T_scalar& s)
: x(coords[0]), y(coords[1])
{
	this->x = s;
	this->y = s;
}

template <class T>
Vector2<T>::Vector2(const Vector2<T>& ref)
: x(coords[0]), y(coords[1])
{
	*this = ref;
}

template <class T>
template <class T_2>
Vector2<T>::Vector2(const Vector2<T_2>& ref)
: x(coords[0]), y(coords[1])
{
	*this = ref;
}

template <class T>
Vector2<T>::~Vector2()
{
}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector2<T>& Vector2<T>::operator=(const Vector2<T_2>& ref)
{
	this->x = ref.x;
	this->y = ref.y;
	return *this;
}

// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector2<T>& Vector2<T>::operator=(const Vector2<T>& ref)
{
	this->x = ref.x;
	this->y = ref.y;
	return *this;
}

// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector2<T>& Vector2<T>::operator=(const T_scalar& s)
{
	this->x = s;
	this->y = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector2<T>& Vector2<T>::operator+=(const Vector2<T_2>& v)
{
	this->x += v.x;
	this->y += v.y;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector2<T>& Vector2<T>::operator-=(const Vector2<T_2>& v)
{
	this->x -= v.x;
	this->y -= v.y;
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector2<T>& Vector2<T>::operator*=(const T_scalar& s)
{
	this->x *= s;
	this->y *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector2<T>& Vector2<T>::operator/=(const T_scalar& s)
{
	this->x /= s;
	this->y /= s;
	return *this;
}

// Addition
template <class T>
template <class T_2>
Vector2<T> Vector2<T>::operator+(const Vector2<T_2>& v) const
{
	return Vector2<T>(this->x + v.x, this->y + v.y);
}

// Soustraction
template <class T>
template <class T_2>
Vector2<T> Vector2<T>::operator-(const Vector2<T_2>& v) const
{
	return Vector2<T>(this->x - v.x, this->y - v.y);
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector2<T> Vector2<T>::operator*(const T_scalar& s) const
{
	return Vector2<T>(this->x * s, this->y * s);
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Vector2<T> Vector2<T>::operator/(const T_scalar& s) const
{
	return Vector2<T>(this->x / s, this->y / s);
}

// Produit scalaire
template <class T>
T Vector2<T>::operator*(const Vector2<T>& v) const
{
	return this->x * v.x + this->y * v.y;
}

// Normalisation + renvoi de la valeur de la norme
template <class T>
double Vector2<T>::normalize()
{
	double norm = sqrtf(x*x + y*y);
	x = T(double(x) / norm);
	y = T(double(y) / norm);

	return norm;
}

// Calcul de la norme
template <class T>
double Vector2<T>::norm() const
{
	return sqrtf(x*x + y*y);
}

// Calcul de la norme au carré (plus rapide)
template <class T>
T Vector2<T>::squaredNorm() const
{
	return x*x + y*y;
}

// Calcul de l'angle
template <class T>
double Vector2<T>::angle() const
{
	return atan2(y, x);
}

// Rotation du vecteur
template <class T>
void Vector2<T>::rotate(double angle)
{
	// On multiplie par la matrice :
	// ( cos(theta), -sin(theta) )
	// ( sin(theta), cos(theta)  )

	double cos_angle = cos(angle);
	double sin_angle = sin(angle);
	double old_x = x;

	x = cos_angle*old_x - sin_angle*y;
	y = sin_angle*old_x + cos_angle*y;
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector2<T> operator*(const T_scalar& s, const Vector2<T>& v)
{
	return Vector2<T>(v.x * s, v.y * s);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector2<T> operator/(const T_scalar& s, const Vector2<T>& v)
{
	return Vector2<T>(v.x / s, v.y / s);
}

// Interpolation
template<class T>
Vector2<T> Vector2<T>::lerp(double factor, const Vector2<T>& vec1, const Vector2<T>& vec2)
{
	return (1-factor)*vec1 + factor*vec2;
}

