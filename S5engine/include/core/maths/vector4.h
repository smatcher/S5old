// Vector4.h
// Vecteur en 4 dimensions
// 2008 par Funto
// Version 1.0

#ifndef VECTOR_4_H
#define VECTOR_4_H

#include <iostream>

template <class T>
class Vector4
{
public:
	T coords[4];

	T& x;
	T& y;
	T& z;
	T& w;

	T& r;
	T& g;
	T& b;
	T& a;

public:
	// Constructeurs
	Vector4();	// par défaut

	Vector4(const T& x, const T& y, const T& z, const T& w);	// à partir des coordonnées

	template <class T_scalar>
	Vector4(const T_scalar& s);	// à partir d'un seul scalaire

	Vector4(const Vector4<T>& ref);	// de copie 1

	template <class T_2>
	Vector4(const Vector4<T_2>& ref);	// de copie 2

	// Destructeur
	virtual ~Vector4();

	// Cast
	operator T* () {return this->coords;}
	operator const T* () const {return (const T*)(this->coords);}

	// Affectation suivant un autre vecteur
	template <class T_2>
	inline Vector4<T>& operator=(const Vector4<T_2>& ref);

	// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
	Vector4<T>& operator=(const Vector4<T>& ref);

	// Affectation suivant un scalaire
	template <class T_scalar>
	inline Vector4<T>& operator=(const T_scalar& s);

	// Affectation-addition
	template <class T_2>
	inline Vector4<T>& operator+=(const Vector4<T_2>& v);

	// Affectation-soustraction
	template <class T_2>
	inline Vector4<T>& operator-=(const Vector4<T_2>& v);

	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	inline Vector4<T>& operator*=(const T_scalar& s);

	// Affectation-division par un scalaire
	template <class T_scalar>
	inline Vector4<T>& operator/=(const T_scalar& s);

	// Addition
	template <class T_2>
	inline Vector4<T> operator+(const Vector4<T_2>& v) const;

	// Soustraction
	template <class T_2>
	inline Vector4<T> operator-(const Vector4<T_2>& v) const;

	// Multiplication par un scalaire (v * s)
	template <class T_scalar>
	inline Vector4<T> operator*(const T_scalar& s) const;

	// Division par un scalaire (v / s)
	template <class T_scalar>
	inline Vector4<T> operator/(const T_scalar& s) const;

	// Produit scalaire
	inline T operator*(const Vector4<T>& v) const;

	// Normalisation + renvoi de la valeur de la norme
	inline double normalize();

	// Calcul de la norme
	inline double norm() const;

	// Calcul de la norme au carré (plus rapide)
	inline T squaredNorm() const;

	// Interpolation
	static Vector4<T> lerp(double factor, const Vector4<T>& vec1, const Vector4<T>& vec2);
};

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector4<T> operator*(const T_scalar& s, const Vector4<T>& v);

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector4<T> operator/(const T_scalar& s, const Vector4<T>& v);

// Fonction pour le produit scalaire
template <class T>
inline T dot(const Vector4<T>& v1, const Vector4<T>& v2)
{
	return v1 * v2;
}

// Affichage avec iostream
template <class T>
inline std::ostream& operator<<(std::ostream& os, const Vector4<T>& v)
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
	return os;
}

typedef Vector4<double>			Vector4d;
typedef Vector4<float>			Vector4f;
typedef Vector4<unsigned int>	Vector4ui;
typedef Vector4<int>			Vector4i;
typedef Vector4<short>			Vector4s;
typedef Vector4<unsigned char>	Vector4ub;

#include "core/maths/vector4.hpp"

#endif	// VECTOR_4_H
