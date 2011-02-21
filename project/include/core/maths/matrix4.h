// Matrix4.h

#ifndef MATRIX4_H
#define MATRIX4_H

#include <iostream>
#include "include/core/maths/vector4.h"
#include "include/core/maths/matrix3.h"

template <class T>
class Matrix4
{
public:
	T values[16];	// NB : les valeurs sont enregistrées "façon OpenGL", i.e. "column-major order" :
					// de l'élément 0 au 3 : c'est la 1ère colonne, etc...

public:
	// Constructeur
	Matrix4();

	// Constructeur à partir de 4 vecteurs
	template <class T_scalar>
	Matrix4(	const Vector4<T_scalar>& v1, const Vector4<T_scalar>& v2,
				const Vector4<T_scalar>& v3, const Vector4<T_scalar>& v4, bool column_vectors=true);

	// Constructeur à partir d'un tableau
	template <class T_scalar>
	Matrix4(	T_scalar* values, bool transpose=false);

	// Constructeur à partir de la sous-matrice 3x3 supérieure gauche et du vecteur de translation
	template <class T_scalar>
	Matrix4(	const Matrix3<T_scalar>& sub_matrix, const Vector3<T_scalar>& translation);

	// Constructeur de copie
	template <class T_scalar>
	Matrix4(const Matrix4<T_scalar>& ref);

	// Destructeur
	virtual ~Matrix4();

	// Cast
	operator T* () {return this->values;}
	operator const T* () const {return (const T*)(this->values);}

	// Opérateur d'affectation
	template <class T_scalar>
	Matrix4<T>& operator=(const Matrix4<T_scalar>& ref);

	// Affectation-addition
	template <class T_scalar>
	Matrix4<T>& operator+=(const Matrix4<T_scalar>& ref);

	// Affectation-soustraction
	template <class T_scalar>
	Matrix4<T>& operator-=(const Matrix4<T_scalar>& ref);

	// Affectation-multiplication
	template <class T_scalar>
	Matrix4<T>& operator*=(const Matrix4<T_scalar>& ref);

	// Affectation-multiplication par un scalaire
	template <class T_scalar>
	Matrix4<T>& operator*=(T_scalar s);

	// Affectation-division par un scalaire
	template <class T_scalar>
	Matrix4<T>& operator /=(T_scalar s);

	// Addition
	template <class T_scalar>
	Matrix4<T> operator+(const Matrix4<T_scalar>& ref) const;

	// Soustraction
	template <class T_scalar>
	Matrix4<T> operator-(const Matrix4<T_scalar>& ref) const;

	// Multiplication
	Matrix4<T> operator*(const Matrix4<T>& ref) const;

	// Multiplication par un scalaire
	template <class T_scalar>
	Matrix4<T> operator*(T_scalar s) const;

	// Multiplication par un vecteur
	Vector4<T> operator*(const Vector4<T>& v) const;

	// Division par un scalaire
	template <class T_scalar>
	Matrix4<T> operator/(T_scalar s) const;

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
	Matrix4<T> getTranspose();
	void transpose();

	// Inversion
	Matrix4<T> getInverse();
	void invert();

	// Transposée de l'inverse
	Matrix4<T> getInverseTranspose();
	void invertAndTranspose();

	// Getters/Setters
	Vector4<T> getRow(int i) const;
	Vector4<T> getColumn(int j) const;

	template <class T_scalar>
	void setRow(const Vector4<T_scalar>& v, int i);

	template <class T_scalar>
	void setColumn(const Vector4<T_scalar>& v, int j);
};

// Multiplication par un scalaire (s * m)
// IMPORTANT : pas de T_scalar, sinon impossible de compiler avec GCC 3 (soit-disant
// que l'ISO est pas d'accord...)
// Donc : le scalaire est du même type que la matrice.
template <class T>
Matrix4<T> operator*(T s, const Matrix4<T>& mat)
{
	return mat * s;
}

// Affichage
template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix4<T>& mat);

typedef Matrix4<int> Matrix4i;
typedef Matrix4<float> Matrix4f;
typedef Matrix4<double> Matrix4d;

#include "include/core/maths/matrix4.hpp"

#endif	// MATRIX4_H

