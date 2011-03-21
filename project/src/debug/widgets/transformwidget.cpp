#include "debug/widgets/transformwidget.h"
#include <QtGui>
#include <QDebug>

TransformWidget::TransformWidget(Transformd &transform) : QGroupBox(tr("Transform")), m_transform(transform)
{
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);

	QLayout* layout = new QVBoxLayout();

	Vector3d pos = m_transform.getPosition();

	m_x = new QLineEdit(QString().setNum(pos.x,'g',2));
	m_x->setDisabled(true);
	m_y = new QLineEdit(QString().setNum(pos.y,'g',2));
	m_y->setDisabled(true);
	m_z = new QLineEdit(QString().setNum(pos.z,'g',2));
	m_z->setDisabled(true);


	Vector3d rot = m_transform.toEuler();
	m_rx = new QLineEdit(QString().setNum(rot.x,'g',2));
	m_rx->setDisabled(true);
	m_ry = new QLineEdit(QString().setNum(rot.y,'g',2));
	m_ry->setDisabled(true);
	m_rz = new QLineEdit(QString().setNum(rot.z,'g',2));
	m_rz->setDisabled(true);

	QGroupBox* posgroup = new QGroupBox(tr("position"));
	QLayout* posgroup_layout = new QHBoxLayout();
	posgroup_layout->addWidget(m_x);
	posgroup_layout->addWidget(m_y);
	posgroup_layout->addWidget(m_z);
	posgroup->setLayout(posgroup_layout);

	QGroupBox* rotgroup = new QGroupBox(tr("rotation"));
	QLayout* rotgroup_layout = new QHBoxLayout();
	rotgroup_layout->addWidget(m_rx);
	rotgroup_layout->addWidget(m_ry);
	rotgroup_layout->addWidget(m_rz);
	rotgroup->setLayout(rotgroup_layout);

	layout->addWidget(posgroup);
	layout->addWidget(rotgroup);
	setLayout(layout);
}

TransformWidget::~TransformWidget()
{

}

QSize TransformWidget::sizeHint() const
{
	return QSize(200,100);
}

void TransformWidget::updateData()
{
	Vector3d pos = m_transform.getPosition();
	m_x->setText(QString().setNum(pos.x,'g',2));
	m_y->setText(QString().setNum(pos.y,'g',2));
	m_z->setText(QString().setNum(pos.z,'g',2));

	Vector3d rot = m_transform.toEuler();
	m_rx->setText(QString().setNum(rot.x,'g',2));
	m_ry->setText(QString().setNum(rot.y,'g',2));
	m_rz->setText(QString().setNum(rot.z,'g',2));
}
