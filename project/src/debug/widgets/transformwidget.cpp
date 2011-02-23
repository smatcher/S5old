#include "include/debug/widgets/transformwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>

TransformWidget::TransformWidget(Transformd &transform) : QGroupBox(tr("Transform")), m_transform(transform)
{
	QLayout* layout = new QVBoxLayout();

	Vector3d pos = m_transform.getPosition();
	QLabel* x = new QLabel(QString().setNum(pos.x));
	QLabel* y = new QLabel(QString().setNum(pos.y));
	QLabel* z = new QLabel(QString().setNum(pos.z));

	Matrix3d rot = m_transform.getRotation();
	QLabel* rx = new QLabel(QString().setNum(rot[0]));
	QLabel* ry = new QLabel(QString().setNum(rot[4]));
	QLabel* rz = new QLabel(QString().setNum(rot[8]));

	QGroupBox* posgroup = new QGroupBox(tr("position"));
	QLayout* posgroup_layout = new QHBoxLayout();
	posgroup_layout->addWidget(x);
	posgroup_layout->addWidget(y);
	posgroup_layout->addWidget(z);
	posgroup->setLayout(posgroup_layout);

	QGroupBox* rotgroup = new QGroupBox(tr("rotation"));
	QLayout* rotgroup_layout = new QHBoxLayout();
	rotgroup_layout->addWidget(rx);
	rotgroup_layout->addWidget(ry);
	rotgroup_layout->addWidget(rz);
	rotgroup->setLayout(rotgroup_layout);

	layout->addWidget(posgroup);
	layout->addWidget(rotgroup);
	setLayout(layout);
}

TransformWidget::~TransformWidget()
{

}
