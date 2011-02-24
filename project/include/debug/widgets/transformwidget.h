#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QGroupBox>
#include "include/core/maths/transform.h"

class TransformWidget : public QGroupBox
{
	Q_OBJECT

public :
	TransformWidget(Transformd& transform);
	~TransformWidget();

	QSize sizeHint() const;

private :
	Transformd m_transform;
};

#endif // TRANSFORMWIDGET_H
