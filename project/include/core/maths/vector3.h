// Vector3.h
// Vecteur en 3 dimensions
// 2008 par Funto
// Version 1.0

#ifndef VECTOR_3_H
#define VECTOR_3_H

#include <iostream>

template <class T>
class Vector3
{
public:
	T coords[3];

	T& x;
	T& y;
	T& z;

	T& r;
	T& g;
	T& b;

public:
	// Constructeurs
	Vector3();	// par défaut

	Vector3(const T& x, const T& y, const T& z);	// à partir des coordonnées

	template <class T_scalar>
	Vector3(const T_scalar& s);	// à partir d'un seul scalaire

	Vector3(const Vector3<T>& ref);	// de copie 1

	template <class T_2>
	Vector3(const Vector3<T_2>& ref);	// de copie 2

	// Destructeur
	virtual ~Vector3();

	// Cast
	operator T* () {return this->coords;}
	operator const T* () const {return (const T*)(this->coords);}

	// Affectation suivant un autre vecteur
	template <class T_2>
	inline Vector3<T>& operator=(const Vector3<T_2>& ref);

	// Affectation suivant un autre vecteur du même type (sinon, ça compile pas...)
	Vector3<T>& operator=(const Vector3<T>& ref);

	// Affectation suivant un scalaire
	template <class T_scalar>
	inline Vector3<T>& operator=(const T_scalar& s);

	// Affectation-addition
	template <class T_2>
	inline Vector3<T>& operator+=(const Vector3<T_2>& v);

	// Affectation-soustraction
	template <class T_2>
	inline Vector3<T>& operator-=(const Vector3<T_2>& v);

	// Affectation-produit vectoriel
	template <class T_2>
	inline Vector3<T>& operator^=(const Vector3<T_2>& v);

	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	inline Vector3<T>& operator*=(const T_scalar& s);

	// Affectation-division par un scalaire
	template <class T_scalar>
	inline Vector3<T>& operator/=(const T_scalar& s);

	// Addition
	template <class T_2>
	inline Vector3<T> operator+(const Vector3<T_2>& v) const;

	// Soustraction
	template <class T_2>
	inline Vector3<T> operator-(const Vector3<T_2>& v) const;

	// Produit vectoriel
	template <class T_2>
	inline Vector3<T> operator^(const Vector3<T_2>& v) const;

	// Multiplication par un scalaire (v * s)
	template <class T_scalar>
	inline Vector3<T> operator*(const T_scalar& s) const;

	// Division par un scalaire (v / s)
	template <class T_scalar>
	inline Vector3<T> operator/(const T_scalar& s) const;

	// Produit scalaire
	inline T operator*(const Vector3<T>& v) const;

	// Normalisation + renvoi de la valeur de la norme
	inline double normalize();

	// Calcul de la norme
	inline double norm() const;

	// Calcul de la norme au carré (plus rapide)
	inline T squaredNorm() const;
};

// Multiplication par un scalaire (s * v)
template <class T, class T_scalar>
Vector3<T> operator*(const T_scalar& s, const Vector3<T>& v);

// Division par un scalaire (s / v)
template <class T, class T_scalar>
Vector3<T> operator/(const T_scalar& s, const Vector3<T>& v);

// Fonction pour le produit scalaire
template <class T>
inline T dot(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1 * v2;
}

// Fonction pour le produit vectoriel
template <class T>
inline Vector3<T> cross(const Vector3<T>& v1, const Vector3<T>& v2)
{
	return v1 ^ v2;
}

// Affichage avec iostream
template <class T>
inline std::ostream& operator<<(std::ostream& os, const Vector3<T>& v)
{
	os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
	return os;
}

typedef Vector3<double>			Vector3d;
typedef Vector3<float>			Vector3f;
typedef Vector3<unsigned int>	Vector3ui;
typedef Vector3<int>			Vector3i;
typedef Vector3<short>			Vector3s;
typedef Vector3<unsigned char>	Vector3ub;

#include "core/maths/vector3.hpp"

#endif	// VECTOR_3_H
