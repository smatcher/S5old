// Matrix3.hpp

// Constructeur
template <class T>
Matrix3<T>::Matrix3()
{
	setIdentity();
}

// Constructeur à partir de 3 vecteurs
template <class T>
template <class T_scalar>
Matrix3<T>::Matrix3(const Vector3<T_scalar>& v1, const Vector3<T_scalar>& v2,
					const Vector3<T_scalar>& v3, bool column_vectors)
{
	if(column_vectors)
	{
		values[0]=v1.x;  values[1]=v1.y;  values[2]=v1.z;
		values[3]=v2.x;  values[4]=v2.y;  values[5]=v2.z;
		values[6]=v3.x;  values[7]=v3.y;  values[8]=v3.z;
	}
	else
	{
		values[0]=v1.x;  values[1]=v2.x;  values[2]=v3.x;
		values[3]=v1.y;  values[4]=v2.y;  values[5]=v3.y;
		values[6]=v1.z;  values[7]=v2.z;  values[8]=v3.z;
	}
}

// Constructeur à partir d'un tableau
template <class T>
template <class T_scalar>
Matrix3<T>::Matrix3(T_scalar* values, bool transpose)
{
	for(int i=0 ; i<9 ; i++)
		this->values[i] = values[i];

	if(transpose)
		this->transpose();
}

// Constructeur à partir d'un quaternion
template <class T>
template <class T_scalar>
Matrix3<T>::Matrix3(const Vector4<T_scalar>& quaternion)
{
	Vector4<T_scalar> _quaternion = quaternion;
	_quaternion.normalize();
	T_scalar x = _quaternion.x;
	T_scalar y = _quaternion.y;
	T_scalar z = _quaternion.z;
	T_scalar w = _quaternion.w;

	this->values[0] = 1.0f - 2.0f * (y * y + z * z);
	this->values[1] = 2.0f * (x * y + z * w);
	this->values[2] = 2.0f * (x * z - y * w);
	this->values[3] = 2.0f * (x * y - z * w);
	this->values[4] = 1.0f - 2.0f * (x * x + z * z);
	this->values[5] = 2.0f * (y * z + x * w);
	this->values[6] = 2.0f * (x * z + y * w);
	this->values[7] = 2.0f * (y * z - x * w);
	this->values[8] = 1.0f - 2.0f * (x * x + y * y);
}

// Constructeur de copie
template <class T>
template <class T_scalar>
Matrix3<T>::Matrix3(const Matrix3<T_scalar>& ref)
{
	*this = ref;
}

// Destructeur
template <class T>
Matrix3<T>::~Matrix3()
{
}

// Opérateur d'affectation
template <class T>
template <class T_scalar>
Matrix3<T>& Matrix3<T>::operator=(const Matrix3<T_scalar>& ref)
{
	for(int i=0 ; i<9 ; i++)
		this->values[i] = ref.values[i];
}

// Affectation-addition
template <class T>
template <class T_scalar>
Matrix3<T>& Matrix3<T>::operator+=(const Matrix3<T_scalar>& ref)
{
	for(int i=0 ; i<9 ; i++)
		this->values[i] += ref.values[i];
}

// Affectation-soustraction
template <class T>
template <class T_scalar>
Matrix3<T>& Matrix3<T>::operator-=(const Matrix3<T_scalar>& ref)
{
	for(int i=0 ; i<9 ; i++)
		this->values[i] -= ref.values[i];
}

// Affectation-multiplication
template <class T>
template <class T_scalar>
Matrix3<T>& Matrix3<T>::operator*=(const Matrix3<T_scalar>& ref)
{
	*this = (*this) * ref;
	return *this;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Matrix3<T>& Matrix3<T>::operator*=(T_scalar s)
{
	*this = (*this) * s;
	return *this;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Matrix3<T>& Matrix3<T>::operator /=(T_scalar s)
{
	*this = (*this) / s;
}

// Addition
template <class T>
template <class T_scalar>
Matrix3<T> Matrix3<T>::operator+(const Matrix3<T_scalar>& ref) const
{
	Matrix3<T> resultat;

	for(int i=0 ; i<9 ; i++)
		resultat.values[i] = this->values[i] + ref.values[i];

	return resultat;
}

// Soustraction
template <class T>
template <class T_scalar>
Matrix3<T> Matrix3<T>::operator-(const Matrix3<T_scalar>& ref) const
{
	Matrix3<T> resultat;

	for(int i=0 ; i<9 ; i++)
		resultat.values[i] = this->values[i] - ref.values[i];

	return resultat;
}

// Multiplication
template <class T>
Matrix3<T> Matrix3<T>::operator*(const Matrix3<T>& ref) const
{
	Matrix3<T> resultat;

	for(int i=0 ; i<3 ; i++)
	{
		for(int j=0 ; j<3 ; j++)
		{
			T temp = T(0);

			for(int k=0 ; k<3 ; k++)
			{
				temp += this->get(i, k) * ref.get(k, j);
			}

			resultat.get(i, j) = temp;
		}
	}

	return resultat;
}

// Multiplication par un scalaire
template <class T>
template <class T_scalar>
Matrix3<T> Matrix3<T>::operator*(T_scalar s) const
{
	Matrix3<T> resultat;

	for(int i=0 ; i<9 ; i++)
		resultat.values[i] = this->values[i] * s;

	return resultat;
}

// Multiplication par un vecteur
template <class T>
Vector3<T> Matrix3<T>::operator*(const Vector3<T>& v) const
{
	Vector3<T> resultat;

	for(int i=0 ; i<3 ; i++)
	{
		resultat[i] = T(0);

		for(int j=0 ; j<3 ; j++)
		{
			resultat[i] += this->get(i, j) * v[j];
		}
	}

	return resultat;
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Matrix3<T> Matrix3<T>::operator/(T_scalar s) const
{
	Matrix3<T> resultat;

	for(int i=0 ; i<9 ; i++)
		resultat.values[i] = this->values[i] / s;

	return resultat;
}

// Accès à un élément
template <class T>
T& Matrix3<T>::operator[](int index)
{
	return values[index];
}

template <class T>
const T& Matrix3<T>::operator[](int index) const
{
	return values[index];
}

template <class T>
T& Matrix3<T>::get(int row, int column)
{
	return values[row + 3*column];
}

template <class T>
const T& Matrix3<T>::get(int row, int column) const
{
	return values[row + 3*column];
}

// Identité
template <class T>
void Matrix3<T>::setIdentity()
{
	values[0]=T(1);  values[1]=T(0);  values[2]=T(0);
	values[3]=T(0);  values[4]=T(1);  values[5]=T(0);
	values[6]=T(0);  values[7]=T(0);  values[8]=T(1);
}

// Zéro
template <class T>
void Matrix3<T>::setZero()
{
	values[0]=T(0);  values[1]=T(0);  values[2]=T(0);
	values[3]=T(0);  values[4]=T(0);  values[5]=T(0);
	values[6]=T(0);  values[7]=T(0);  values[8]=T(0);
}

// Calcul du déterminant
template <class T>
T Matrix3<T>::determinant()
{
	return	+ values[0] * (values[1]*values[5] - values[2]*values[4])
			- values[1] * (values[3]*values[8] - values[5]*values[6])
			+ values[2] * (values[3]*values[7] - values[4]*values[6]);
}

// Renvoi de la matrice transposée
template <class T>
Matrix3<T> Matrix3<T>::getTranspose()
{
	Matrix3<T> result = *this;
	result.transpose();
	return result;
}

// Transposition
template <class T>
void Matrix3<T>::transpose()
{
	T temp;
	temp=values[1];  values[1]=values[3];  values[3]=temp;
	temp=values[2];  values[2]=values[6];  values[6]=temp;
	temp=values[5];  values[5]=values[7];  values[7]=temp;
}

// Renvoi de l'inverse de la matrice
template <class T>
Matrix3<T> Matrix3<T>::getInverseTranspose()
{
	Matrix3<T> result = *this;
	result.invertAndTranspose();
	return result;
}

// Inversion
template <class T>
void Matrix3<T>::invert()
{
	*this = getInverse();
}

// Renvoi de la transposée de l'inverse de la matrice
template <class T>
Matrix3<T> Matrix3<T>::getInverse()
{
	T& m11=values[0];
	T& m21=values[1];
	T& m31=values[2];

	T& m12=values[3];
	T& m22=values[4];
	T& m32=values[5];

	T& m13=values[6];
	T& m23=values[7];
	T& m33=values[8];

	Matrix3<T> result(
		Vector3<T>((m22*m33 - m23*m32), -(m12*m33 - m13*m32),  (m12*m23 - m13*m22)),
		Vector3<T>(-(m21*m33 - m23*m31),  (m11*m33 - m13*m31), -(m11*m23 - m13*m21)),
		Vector3<T>((m21*m32 - m22*m31), -(m11*m32 - m12*m31),  (m11*m22 - m12*m21)),false);

	T det = m11*result.values[0] + m12*result.values[1] + m13*result.values[2];
	return (result/det);
}

// Calcul de la transposée de l'inverse - méthode repompée du code de Paul Baker
template <class T>
void Matrix3<T>::invertAndTranspose()
{
	this->invert();
	this->transpose();
}

// Getters/Setters
template <class T>
Vector3<T> Matrix3<T>::getRow(int i) const
{
	return Vector3<T>(values[i + 0*3], values[i + 1*3], values[i + 2*3]);
}

template <class T>
Vector3<T> Matrix3<T>::getColumn(int j) const
{
	return Vector3<T>(values[j*3 + 0], values[j*3 + 1], values[j*3 + 2]);
}

template <class T>
template <class T_scalar>
void Matrix3<T>::setRow(const Vector3<T_scalar>& v, int i)
{
	values[i + 0*3] = v.x;
	values[i + 1*3] = v.y;
	values[i + 2*3] = v.z;
}

template <class T>
template <class T_scalar>
void Matrix3<T>::setColumn(const Vector3<T_scalar>& v, int j)
{
	values[j*3 + 0] = v.x;
	values[j*3 + 1] = v.y;
	values[j*3 + 2] = v.z;
}

// Affichage
template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix3<T>& mat)
{
	Vector3<T> r1 = mat.getRow(0);
	Vector3<T> r2 = mat.getRow(1);
	Vector3<T> r3 = mat.getRow(2);

	os << r1 << std::endl << r2 << std::endl << r3 << std::endl;

	return os;
}
