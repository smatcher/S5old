// Matrix3.h

#ifndef MATRIX3_H
#define MATRIX3_H

#include <iostream>
#include "core/maths/vector3.h"
#include "core/maths/vector4.h"

template <class T>
class Matrix3
{
public:
	T values[9];	// NB : les valeurs sont enregistrées "façon OpenGL", i.e. "column-major order" :
					// de l'élément 0 au 2 : c'est la 1ère colonne, etc...

public:
	// Constructeur
	Matrix3();

	// Constructeur à partir de 3 vecteurs
	template <class T_scalar>
	Matrix3(	const Vector3<T_scalar>& v1,
				const Vector3<T_scalar>& v2,
				const Vector3<T_scalar>& v3, bool column_vectors=true);

	// Constructeur à partir d'un tableau
	template <class T_scalar>
	Matrix3(T_scalar* values, bool transpose=false);

	// Constructeur à partir d'un quaternion
	template <class T_scalar>
	Matrix3(const Vector4<T_scalar>& quaternion);

	// Constructeur de copie
	template <class T_scalar>
	Matrix3(const Matrix3<T_scalar>& ref);

	// Destructeur
	virtual ~Matrix3();

	// Cast
	operator T* () {return this->values;}
	operator const T* () const {return (const T*)(this->values);}

	// Opérateur d'affectation
	template <class T_scalar>
	Matrix3<T>& operator=(const Matrix3<T_scalar>& ref);

	// Affectation-addition
	template <class T_scalar>
	Matrix3<T>& operator+=(const Matrix3<T_scalar>& ref);

	// Affectation-soustraction
	template <class T_scalar>
	Matrix3<T>& operator-=(const Matrix3<T_scalar>& ref);

	// Affectation-multiplication
	template <class T_scalar>
	Matrix3<T>& operator*=(const Matrix3<T_scalar>& ref);

	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	Matrix3<T>& operator*=(T_scalar s);

	// Affectation-division par un scalaire
	template <class T_scalar>
	Matrix3<T>& operator /=(T_scalar s);

	// Addition
	template <class T_scalar>
	Matrix3<T> operator+(const Matrix3<T_scalar>& ref) const;

	// Soustraction
	template <class T_scalar>
	Matrix3<T> operator-(const Matrix3<T_scalar>& ref) const;

	// Multiplication
	Matrix3<T> operator*(const Matrix3<T>& ref) const;

	// Multiplication par un scalaire
	template <class T_scalar>
	Matrix3<T> operator*(T_scalar s) const;

	// Multiplication par un vecteur
	Vector3<T> operator*(const Vector3<T>& v) const;

	// Division par un scalaire
	template <class T_scalar>
	Matrix3<T> operator/(T_scalar s) const;

	// Accès à un élément
	T& operator[](int index);
	const T& operator[](int index) const;

	T& get(int row, int column);
	const T& get(int row, int column) const;

	// Identité
	void setIdentity();

	// Zéro
	void setZero();

	T determinant();

	// Transposition
	Matrix3<T> getTranspose();
	void transpose();

	// Inversion
	Matrix3<T> getInverse();
	void invert();

	// Transposée de l'inverse
	Matrix3<T> getInverseTranspose();
	void invertAndTranspose();

	// Getters/Setters
	Vector3<T> getRow(int i) const;
	Vector3<T> getColumn(int j) const;

	template <class T_scalar>
	void setRow(const Vector3<T_scalar>& v, int i);

	template <class T_scalar>
	void setColumn(const Vector3<T_scalar>& v, int j);
};

// Multiplication par un scalaire (s * m)
// IMPORTANT : pas de T_scalar, sinon impossible de compiler avec GCC 3 (soit-disant
// que l'ISO est pas d'accord...)
// Donc : le scalaire est du même type que la matrice.
template <class T>
Matrix3<T> operator*(T s, const Matrix3<T>& mat)
{
	return mat * s;
}

// Affichage
template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix3<T>& mat);

typedef Matrix3<int> Matrix3i;
typedef Matrix3<float> Matrix3f;
typedef Matrix3<double> Matrix3d;

#include "core/maths/matrix3.hpp"

#endif	// MATRIX3_H
