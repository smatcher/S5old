#include "include/debug/widgets/transformwidget.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include <QDoubleValidator>
#include <QSizePolicy>

TransformWidget::TransformWidget(Transformd &transform) : QGroupBox(tr("Transform")), m_transform(transform)
{
	setSizePolicy(QSizePolicy::Maximum,QSizePolicy::Maximum);

	QLayout* layout = new QVBoxLayout();

	Vector3d pos = m_transform.getPosition();
	/*
	QLabel* x = new QLabel(QString().setNum(pos.x,'g',2));
	QLabel* y = new QLabel(QString().setNum(pos.y,'g',2));
	QLabel* z = new QLabel(QString().setNum(pos.z,'g',2));
	*/

	QLineEdit* x = new QLineEdit(QString().setNum(pos.x,'g',2));
	x->setDisabled(true);
	QLineEdit* y = new QLineEdit(QString().setNum(pos.y,'g',2));
	y->setDisabled(true);
	QLineEdit* z = new QLineEdit(QString().setNum(pos.z,'g',2));
	z->setDisabled(true);


	Matrix3d rot = m_transform.getRotation();
	QLineEdit* rx = new QLineEdit(QString().setNum(rot[0],'g',2));
	rx->setDisabled(true);
	QLineEdit* ry = new QLineEdit(QString().setNum(rot[4],'g',2));
	ry->setDisabled(true);
	QLineEdit* rz = new QLineEdit(QString().setNum(rot[8],'g',2));
	rz->setDisabled(true);

	/*
	QLineEdit* rx = new QLineEdit(QString().setNum(rot[0]));
	rx->setValidator(new QDoubleValidator(rx));
	QLineEdit* ry = new QLineEdit(QString().setNum(rot[4]));
	ry->setValidator(new QDoubleValidator(ry));
	QLineEdit* rz = new QLineEdit(QString().setNum(rot[8]));
	rz->setValidator(new QDoubleValidator(rz));
	*/
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
