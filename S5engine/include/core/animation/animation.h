#ifndef ANIMATION_H
#define ANIMATION_H

#include <QVector>
#include "core/maths/vector3.h"
#include "core/maths/matrix3.h"

class AssimpFactory;

class Animation
{
	friend class AssimpFactory;

public:

	template <class T>
	struct AnimKey {
		double time;
		T value;
	};

private:
	QVector< AnimKey<Vector3f> > m_translationKeys;
	QVector< AnimKey<Matrix3f> > m_rotationKeys;
	QVector< AnimKey<Vector3f> > m_scalingKeys;
};

#endif //ANIMATION_H
