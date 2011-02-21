// Vector4.hpp
// Vecteur en 4 dimensions
// 2008 par Funto
// Version 1.0

#include <math.h>

template <class T>
Vector4<T>::Vector4()
: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]),
  r(coords[0]), g(coords[1]), b(coords[2]), a(coords[3])
{
	x = T(0);
	y = T(0);
	z = T(0);
	w = T(1);	// MEME pour les Vector4ub (Cf juste en-dessous...)
}

// TODO : on ne laisse pas cette partie-là parce que sinon, erreur de link avec GCC 4
// lorsque 2 fichiers .cpp incluent Vector4.h...
// cas particulier pour les unsigned char : w = 255
//template <>
//Vector4<unsigned char>::Vector4()
//: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]),
//  r(coords[0]), g(coords[1]), b(coords[2]), a(coords[3])
//{
//	x = 0x00;
//	y = 0x00;
//	z = 0x00;
//	w = 0xFF;
//}

template <class T>
Vector4<T>::Vector4(const T& x, const T& y, const T& z, const T& w)
: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]),
  r(coords[0]), g(coords[1]), b(coords[2]), a(coords[3])
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->w = w;
}

template <class T>
template <class T_scalar>
Vector4<T>::Vector4(const T_scalar& s)
: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]),
  r(coords[0]), g(coords[1]), b(coords[2]), a(coords[3])
{
	this->x = s;
	this->y = s;
	this->z = s;
	this->w = s;
}

template <class T>
Vector4<T>::Vector4(const Vector4<T>& ref)
: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]),
  r(coords[0]), g(coords[1]), b(coords[2]), a(coords[3])
{
	*this = ref;
}

template <class T>
template <class T_2>
Vector4<T>::Vector4(const Vector4<T_2>& ref)
: x(coords[0]), y(coords[1]), z(coords[2]), w(coords[3]),
  r(coords[0]), g(coords[1]), b(coords[2]), a(coords[3])
{
	*this = ref;
}

template <class T>
Vector4<T>::~Vector4()
{
}

// Affectation suivant un autre vecteur
template <class T>
template <class T_2>
Vector4<T>& Vector4<T>::operator=(const Vector4<T_2>& ref)
{
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	this->w = ref.w;
	return *this;
}


// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
template <class T>
Vector4<T>& Vector4<T>::operator=(const Vector4<T>& ref)
{
	this->x = ref.x;
	this->y = ref.y;
	this->z = ref.z;
	this->w = ref.w;
	return *this;
}


// Affectation suivant un scalaire
template <class T>
template <class T_scalar>
Vector4<T>& Vector4<T>::operator=(const T_scalar& s)
{
	this->x = s;
	this->y = s;
	this->z = s;
	this->w = s;
	return *this;
}

// Affectation-addition
template <class T>
template <class T_2>
Vector4<T>& Vector4<T>::operator+=(const Vector4<T_2>& v)
{
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
	this->w += v.w;
	return *this;
}

// Affectation-soustraction
template <class T>
template <class T_2>
Vector4<T>& Vector4<T>::operator-=(const Vector4<T_2>& v)
{
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
	this->w -= v.w;
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector4<T>& Vector4<T>::operator*=(const T_scalar& s)
{
	this->x *= s;
	this->y *= s;
	this->z *= s;
	this->w *= s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Vector4<T>& Vector4<T>::operator/=(const T_scalar& s)
{
	this->x /= s;
	this->y /= s;
	this->z /= s;
	this->w /= s;
	return *this;
}

// Addition
template <class T>
template <class T_2>
Vector4<T> Vector4<T>::operator+(const Vector4<T_2>& v) const
{
	return Vector4<T>(this->x + v.x, this->y + v.y, this->z + v.z, this->w + v.w);
}

// Soustraction
template <class T>
template <class T_2>
Vector4<T> Vector4<T>::operator-(const Vector4<T_2>& v) const
{
	return Vector4<T>(this->x - v.x, this->y - v.y, this->z - v.z, this->w - v.w);
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Vector4<T> Vector4<T>::operator*(const T_scalar& s) const
{
	return Vector4<T>(this->x * s, this->y * s, this->z * s, this->w * s);
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Vector4<T> Vector4<T>::operator/(const T_scalar& s) const
{
	return Vector4<T>(this->x / s, this->y / s, this->z / s, this->w / s);
}

// Produit scalaire
template <class T>
T Vector4<T>::operator*(const Vector4<T>& v) const
{
	return this->x * v.x + this->y * v.y + this->z * v.z + this->w * v.w;
}

// Normalisation + renvoi de la valeur de la norme
template <class T>
double Vector4<T>::normalize()
{
	double norm = sqrtf(x*x + y*y + z*z + w*w);
	x = T(double(x) / norm);
	y = T(double(y) / norm);
	z = T(double(z) / norm);
	w = T(double(w) / norm);

	return norm;
}

// Calcul de la norme
template <class T>
double Vector4<T>::norm() const
{
	return sqrtf(x*x + y*y + z*z + w*w);
}

// Calcul de la norme au carré (plus rapide)
template <class T>
T Vector4<T>::squaredNorm() const
{
	return x*x + y*y + z*z + w*w;
}

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector4<T> operator*(const T_scalar& s, const Vector4<T>& v)
{
	return Vector4<T>(v.x * s, v.y * s, v.z * s, v.w * s);
}

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector4<T> operator/(const T_scalar& s, const Vector4<T>& v)
{
	return Vector4<T>(v.x / s, v.y / s, v.z / s, v.w / s);
}
