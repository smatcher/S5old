// Matrix4.hpp

// Constructeur
template <class T>
Matrix4<T>::Matrix4()
{
	setIdentity();
}

// Constructeur à partir de 4 vecteurs
template <class T>
template <class T_scalar>
Matrix4<T>::Matrix4(const Vector4<T_scalar>& v1, const Vector4<T_scalar>& v2,
						const Vector4<T_scalar>& v3, const Vector4<T_scalar>& v4, bool column_vectors)
{
	if(column_vectors)
	{
		values[0]=v1.x;  values[1]=v1.y;  values[2]=v1.z;  values[3]=v1.w;
		values[4]=v2.x;  values[5]=v2.y;  values[6]=v2.z;  values[7]=v2.w;
		values[8]=v3.x;  values[9]=v3.y;  values[10]=v3.z; values[11]=v3.w;
		values[12]=v4.x; values[13]=v4.y; values[14]=v4.z; values[15]=v4.w;
	}
	else
	{
		values[0]=v1.x;  values[1]=v2.x;  values[2]=v3.x;  values[3]=v4.x;
		values[4]=v1.y;  values[5]=v2.y;  values[6]=v3.y;  values[7]=v4.y;
		values[8]=v1.z;  values[9]=v2.z;  values[10]=v3.z; values[11]=v4.z;
		values[12]=v1.w; values[13]=v2.w; values[14]=v3.w; values[15]=v4.w;
	}
}

// Constructeur à partir d'un tableau
template <class T>
template <class T_scalar>
Matrix4<T>::Matrix4(T_scalar* values, bool transpose)
{
	for(int i=0 ; i<16 ; i++)
		this->values[i] = values[i];

	if(transpose)
		this->transpose();
}

// Constructeur à partir de la sous-matrice 3x3 supérieure gauche et du vecteur de translation
template <class T>
template <class T_scalar>
Matrix4<T>::Matrix4(	const Matrix3<T_scalar>& sub_matrix, const Vector3<T_scalar>& translation)
{
	this->values[0] = sub_matrix[0];
	this->values[1] = sub_matrix[1];
	this->values[2] = sub_matrix[2];
	this->values[3] = T(0);
	this->values[4] = sub_matrix[3];
	this->values[5] = sub_matrix[4];
	this->values[6] = sub_matrix[5];
	this->values[7] = T(0);
	this->values[8] = sub_matrix[6];
	this->values[9] = sub_matrix[7];
	this->values[10] = sub_matrix[8];
	this->values[11] = T(0);

	this->values[12] = translation.x;
	this->values[13] = translation.y;
	this->values[14] = translation.z;
	this->values[15] = T(1);
}

// Constructeur de copie
template <class T>
template <class T_scalar>
Matrix4<T>::Matrix4(const Matrix4<T_scalar>& ref)
{
	*this = ref;
}

// Destructeur
template <class T>
Matrix4<T>::~Matrix4()
{
}

// Opérateur d'affectation
template <class T>
template <class T_scalar>
Matrix4<T>& Matrix4<T>::operator=(const Matrix4<T_scalar>& ref)
{
	for(int i=0 ; i<16 ; i++)
		this->values[i] = ref.values[i];
}

// Affectation-addition
template <class T>
template <class T_scalar>
Matrix4<T>& Matrix4<T>::operator+=(const Matrix4<T_scalar>& ref)
{
	for(int i=0 ; i<16 ; i++)
		this->values[i] += ref.values[i];
}

// Affectation-soustraction
template <class T>
template <class T_scalar>
Matrix4<T>& Matrix4<T>::operator-=(const Matrix4<T_scalar>& ref)
{
	for(int i=0 ; i<16 ; i++)
		this->values[i] -= ref.values[i];
}

// Affectation-multiplication
template <class T>
template <class T_scalar>
Matrix4<T>& Matrix4<T>::operator*=(const Matrix4<T_scalar>& ref)
{
	*this = (*this) * ref;
}

// Affectation-multiplication par un scalaire
template <class T>
template <class T_scalar>
Matrix4<T>& Matrix4<T>::operator*=(T_scalar s)
{
	*this = (*this) * s;
}

// Affectation-division par un scalaire
template <class T>
template <class T_scalar>
Matrix4<T>& Matrix4<T>::operator /=(T_scalar s)
{
	*this = (*this) / s;
}

// Addition
template <class T>
template <class T_scalar>
Matrix4<T> Matrix4<T>::operator+(const Matrix4<T_scalar>& ref) const
{
	Matrix4<T> resultat;

	for(int i=0 ; i<16 ; i++)
		resultat.values[i] = this->values[i] + ref.values[i];

	return resultat;
}

// Soustraction
template <class T>
template <class T_scalar>
Matrix4<T> Matrix4<T>::operator-(const Matrix4<T_scalar>& ref) const
{
	Matrix4<T> resultat;

	for(int i=0 ; i<16 ; i++)
		resultat.values[i] = this->values[i] - ref.values[i];

	return resultat;
}

// Multiplication
template <class T>
Matrix4<T> Matrix4<T>::operator*(const Matrix4<T>& ref) const
{
	Matrix4<T> resultat;

	for(int i=0 ; i<4 ; i++)
	{
		for(int j=0 ; j<4 ; j++)
		{
			T temp = T(0);

			for(int k=0 ; k<4 ; k++)
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
Matrix4<T> Matrix4<T>::operator*(T_scalar s) const
{
	Matrix4<T> resultat;

	for(int i=0 ; i<16 ; i++)
		resultat.values[i] = this->values[i] * s;

	return resultat;
}

// Multiplication par un vecteur
template <class T>
Vector4<T> Matrix4<T>::operator*(const Vector4<T>& v) const
{
	Vector4<T> resultat;

	for(int i=0 ; i<4 ; i++)
	{
		resultat[i] = T(0);

		for(int j=0 ; j<4 ; j++)
		{
			resultat[i] += this->get(i, j) * v[j];
		}
	}

	return resultat;
}

// Division par un scalaire
template <class T>
template <class T_scalar>
Matrix4<T> Matrix4<T>::operator/(T_scalar s) const
{
	Matrix4<T> resultat;

	for(int i=0 ; i<16 ; i++)
		resultat.values[i] = this->values[i] / s;

	return resultat;
}

// Accès à un élément
template <class T>
T& Matrix4<T>::operator[](int index)
{
	return values[index];
}

template <class T>
const T& Matrix4<T>::operator[](int index) const
{
	return values[index];
}

template <class T>
T& Matrix4<T>::get(int row, int column)
{
	return values[row + 4*column];
}

template <class T>
const T& Matrix4<T>::get(int row, int column) const
{
	return values[row + 4*column];
}

// Identité
template <class T>
void Matrix4<T>::setIdentity()
{
	values[0]=T(1);  values[1]=T(0);  values[2]=T(0);  values[3]=T(0);
	values[4]=T(0);  values[5]=T(1);  values[6]=T(0);  values[7]=T(0);
	values[8]=T(0);  values[9]=T(0);  values[10]=T(1); values[11]=T(0);
	values[12]=T(0); values[13]=T(0); values[14]=T(0); values[15]=T(1);
}

// Zéro
template <class T>
void Matrix4<T>::setZero()
{
	values[0]=T(0);  values[1]=T(0);  values[2]=T(0);  values[3]=T(0);
	values[4]=T(0);  values[5]=T(0);  values[6]=T(0);  values[7]=T(0);
	values[8]=T(0);  values[9]=T(0);  values[10]=T(0); values[11]=T(0);
	values[12]=T(0); values[13]=T(0); values[14]=T(0); values[15]=T(0);
}

// Calcul du déterminant
template <class T>
T Matrix4<T>::determinant()
{
	T result = T(0.0);
	T det3 = T(0.0);

	// On décompose en 4 déterminants 3x3, multipliés par +/- ai, avec ai : ième terme de la 1ère ligne.

	det3 =	+ values[5]  * (values[10]*values[15] - values[14]*values[11])
			- values[9]  * (values[6]*values[15] - values[14]*values[7])
			+ values[13] * (values[6]*values[11] - values[10]*values[7]);
	result += values[0] * det3;

	det3 =	+ values[1]  * (values[10]*values[15] - values[14]*values[11])
			- values[9]  * (values[2]*values[15] - values[14]*values[3])
			+ values[13] * (values[2]*values[11] - values[10]*values[3]);
	result -= values[4] * det3;

	det3 =	+ values[1]  * (values[6]*values[15] - values[14]*values[7])
			- values[5]  * (values[2]*values[15] - values[14]*values[3])
			+ values[13] * (values[2]*values[7] - values[6]*values[3]);
	result += values[8] * det3;

	det3 =	+ values[1]  * (values[6]*values[11] - values[10]*values[7])
			- values[5]  * (values[2]*values[11] - values[10]*values[3])
			+ values[9]  * (values[2]*values[7] - values[6]*values[3]);
	result -= values[12] * det3;

	return result;
}

// Renvoi de la matrice transposée
template <class T>
Matrix4<T> Matrix4<T>::getTranspose() const
{
	Matrix4<T> result = *this;
	result.transpose();
	return result;
}

// Transposition
template <class T>
void Matrix4<T>::transpose()
{
	T temp;
	temp=values[1];  values[1]=values[4];  values[4]=temp;
	temp=values[2];  values[2]=values[8];  values[8]=temp;
	temp=values[3];  values[3]=values[12]; values[12]=temp;
	temp=values[6];  values[6]=values[9];  values[9]=temp;
	temp=values[7];  values[7]=values[13]; values[13]=temp;
	temp=values[11]; values[11]=values[14]; values[14]=temp;
}

// Renvoi de l'inverse de la matrice
template <class T>
Matrix4<T> Matrix4<T>::getInverseTranspose() const
{
	Matrix4<T> result = *this;
	result.invertAndTranspose();
	return result;
}

// Inversion
template <class T>
void Matrix4<T>::invert()
{
	this->invertAndTranspose();
	this->transpose();
}

// Renvoi de la transposée de l'inverse de la matrice
template <class T>
Matrix4<T> Matrix4<T>::getInverse() const
{
	Matrix4<T> result = *this;
	result.invert();
	return result;
}

// Calcul de la transposée de l'inverse - méthode repompée du code de Paul Baker
template <class T>
void Matrix4<T>::invertAndTranspose()
{
	Matrix4<T> comatrix_transpose;
	T pairs[12];
	T det = T(0.0);

	// Calcul de valeurs temporaires pour les 8 1ers éléments
	pairs[0] = values[10] * values[15];
	pairs[1] = values[11] * values[14];
	pairs[2] = values[9] * values[15];
	pairs[3] = values[11] * values[13];
	pairs[4] = values[9] * values[14];
	pairs[5] = values[10] * values[13];
	pairs[6] = values[8] * values[15];
	pairs[7] = values[11] * values[12];
	pairs[8] = values[8] * values[14];
	pairs[9] = values[10] * values[12];
	pairs[10] = values[8] * values[13];
	pairs[11] = values[9] * values[12];

	// Calcul des 8 1ers éléments (cofacteurs)
	comatrix_transpose.values[0] = pairs[0]*values[5] + pairs[3]*values[6] + pairs[4]*values[7]
								- pairs[1]*values[5] - pairs[2]*values[6] - pairs[5]*values[7];

	comatrix_transpose.values[1] = pairs[1]*values[4] + pairs[6]*values[6] + pairs[9]*values[7]
								- pairs[0]*values[4] - pairs[7]*values[6] - pairs[8]*values[7];

	comatrix_transpose.values[2] = pairs[2]*values[4] + pairs[7]*values[5] + pairs[10]*values[7]
								- pairs[3]*values[4] - pairs[6]*values[5] - pairs[11]*values[7];

	comatrix_transpose.values[3] = pairs[5]*values[4] + pairs[8]*values[5] + pairs[11]*values[6]
								- pairs[4]*values[4] - pairs[9]*values[5] - pairs[10]*values[6];

	comatrix_transpose.values[4] = pairs[1]*values[1] + pairs[2]*values[2] + pairs[5]*values[3]
								-	pairs[0]*values[1] - pairs[3]*values[2] - pairs[4]*values[3];

	comatrix_transpose.values[5] = pairs[0]*values[0] + pairs[7]*values[2] + pairs[8]*values[3]
								-	pairs[1]*values[0] - pairs[6]*values[2] - pairs[9]*values[3];

	comatrix_transpose.values[6] = pairs[3]*values[0] + pairs[6]*values[1] + pairs[11]*values[3]
								-	pairs[2]*values[0] - pairs[7]*values[1] - pairs[10]*values[3];

	comatrix_transpose.values[7] = pairs[4]*values[0] + pairs[9]*values[1] + pairs[10]*values[2]
								-	pairs[5]*values[0] - pairs[8]*values[1] - pairs[11]*values[2];

	// Calcul de valeurs temporaires pour les 8 éléments suivants
	pairs[0] = values[2]*values[7];
	pairs[1] = values[3]*values[6];
	pairs[2] = values[1]*values[7];
	pairs[3] = values[3]*values[5];
	pairs[4] = values[1]*values[6];
	pairs[5] = values[2]*values[5];
	pairs[6] = values[0]*values[7];
	pairs[7] = values[3]*values[4];
	pairs[8] = values[0]*values[6];
	pairs[9] = values[2]*values[4];
	pairs[10] = values[0]*values[5];
	pairs[11] = values[1]*values[4];

	// Calcul des 8 éléments suivants (cofacteurs)
	comatrix_transpose.values[8] = pairs[0]*values[13] + pairs[3]*values[14] + pairs[4]*values[15]
								-	pairs[1]*values[13] - pairs[2]*values[14] - pairs[5]*values[15];

	comatrix_transpose.values[9] = pairs[1]*values[12] + pairs[6]*values[14] + pairs[9]*values[15]
								-	pairs[0]*values[12] - pairs[7]*values[14] - pairs[8]*values[15];

	comatrix_transpose.values[10] = pairs[2]*values[12] + pairs[7]*values[13] + pairs[10]*values[15]
								-	pairs[3]*values[12] - pairs[6]*values[13] - pairs[11]*values[15];

	comatrix_transpose.values[11] = pairs[5]*values[12] + pairs[8]*values[13] + pairs[11]*values[14]
								-	pairs[4]*values[12] - pairs[9]*values[13] - pairs[10]*values[14];

	comatrix_transpose.values[12] = pairs[2]*values[10] + pairs[5]*values[11] + pairs[1]*values[9]
								-	pairs[4]*values[11] - pairs[0]*values[9] - pairs[3]*values[10];

	comatrix_transpose.values[13] = pairs[8]*values[11] + pairs[0]*values[8] + pairs[7]*values[10]
								-	pairs[6]*values[10] - pairs[9]*values[11] - pairs[1]*values[8];

	comatrix_transpose.values[14] = pairs[6]*values[9] + pairs[11]*values[11] + pairs[3]*values[8]
								-	pairs[10]*values[11] - pairs[2]*values[8] - pairs[7]*values[9];

	comatrix_transpose.values[15] = pairs[10]*values[10] + pairs[4]*values[8] + pairs[9]*values[9]
					-	pairs[8]*values[9] - pairs[11]*values[10] - pairs[5]*values[8];

	// Calcul du déterminant (en réutilisant les valeurs calculées pour la comatrice)
	det	=	values[0]*comatrix_transpose.values[0]
			+values[1]*comatrix_transpose.values[1]
			+values[2]*comatrix_transpose.values[2]
			+values[3]*comatrix_transpose.values[3];

	*this = comatrix_transpose / det;
}

// Getters/Setters
template <class T>
Vector4<T> Matrix4<T>::getRow(int i) const
{
	return Vector4<T>(values[i + 0*4], values[i + 1*4], values[i + 2*4], values[i + 3*4]);
}

template <class T>
Vector4<T> Matrix4<T>::getColumn(int j) const
{
	return Vector4<T>(values[j*4 + 0], values[j*4 + 1], values[j*4 + 2], values[j*4 + 3]);
}

template <class T>
template <class T_scalar>
void Matrix4<T>::setRow(const Vector4<T_scalar>& v, int i)
{
	values[i + 0*4] = v.x;
	values[i + 1*4] = v.y;
	values[i + 2*4] = v.z;
	values[i + 3*4] = v.w;
}

template <class T>
template <class T_scalar>
void Matrix4<T>::setColumn(const Vector4<T_scalar>& v, int j)
{
	values[j*4 + 0] = v.x;
	values[j*4 + 1] = v.y;
	values[j*4 + 2] = v.z;
	values[j*4 + 3] = v.w;
}

// Affichage
template <class T>
std::ostream& operator<<(std::ostream& os, const Matrix4<T>& mat)
{
	Vector4<T> r1 = mat.getRow(0);
	Vector4<T> r2 = mat.getRow(1);
	Vector4<T> r3 = mat.getRow(2);
	Vector4<T> r4 = mat.getRow(3);

	os << r1 << std::endl << r2 << std::endl << r3 << std::endl << r4 << std::endl;

	return os;
}
