#ifndef ANIMATION_H
#define ANIMATION_H

#include <QVector>
#include <QString>
#include "core/maths/vector3.h"
#include "core/maths/matrix3.h"

class AssimpFactory;

class Animation
{
public:

	template <class T>
	struct AnimKey {
		double time;
		T value;
	};

	struct AnimChannel {
		QString m_name;
		QVector< AnimKey<Vector3f> > m_translation_keys;
		QVector< AnimKey<Matrix3f> > m_rotation_keys;
		QVector< AnimKey<Vector3f> > m_scaling_keys;
	};

	QString m_name;
	double m_duration;
	QVector<AnimChannel> m_channels;
};

#endif //ANIMATION_H
