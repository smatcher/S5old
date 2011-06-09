#ifndef TRANSFORMWIDGET_H
#define TRANSFORMWIDGET_H

#include <QGroupBox>
#include <QLineEdit>
#include "core/maths/transform.h"

class TransformWidget : public QGroupBox
{
	Q_OBJECT

public :
	TransformWidget(Transformf& transform);
	virtual ~TransformWidget();

	QSize sizeHint() const;

	virtual void updateData();

private :
	Transformf& m_transform;

	QLineEdit *m_x,  *m_y,  *m_z;
	QLineEdit *m_rx, *m_ry, *m_rz;
};

#endif // TRANSFORMWIDGET_H
