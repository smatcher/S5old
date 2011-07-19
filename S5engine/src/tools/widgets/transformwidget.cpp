#include "tools/widgets/transformwidget.h"
#include <QtGui>

TransformWidget::TransformWidget(Transformf &transform) : QGroupBox(tr("Transform")), m_transform(transform)
{
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);

	QLayout* layout = new QVBoxLayout();

	Vector3f pos = m_transform.getPosition();

	m_x = new QLineEdit(QString().setNum(pos.x,'g',3));
	m_x->setDisabled(true);
	m_y = new QLineEdit(QString().setNum(pos.y,'g',3));
	m_y->setDisabled(true);
	m_z = new QLineEdit(QString().setNum(pos.z,'g',3));
	m_z->setDisabled(true);


	Vector3f rot = m_transform.toEuler();
	m_rx = new QLineEdit(QString().setNum(rot.x,'g',3));
	m_rx->setDisabled(true);
	m_ry = new QLineEdit(QString().setNum(rot.y,'g',3));
	m_ry->setDisabled(true);
	m_rz = new QLineEdit(QString().setNum(rot.z,'g',3));
	m_rz->setDisabled(true);

	Vector3f scale = m_transform.getScale();
	m_sx = new QLineEdit(QString().setNum(scale.x,'g',3));
	m_sx->setDisabled(true);
	m_sy = new QLineEdit(QString().setNum(scale.y,'g',3));
	m_sy->setDisabled(true);
	m_sz = new QLineEdit(QString().setNum(scale.z,'g',3));
	m_sz->setDisabled(true);

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

	QGroupBox* scgroup = new QGroupBox(tr("scaling"));
	QLayout* scgroup_layout = new QHBoxLayout();
	scgroup_layout->addWidget(m_sx);
	scgroup_layout->addWidget(m_sy);
	scgroup_layout->addWidget(m_sz);
	scgroup->setLayout(scgroup_layout);

	layout->addWidget(posgroup);
	layout->addWidget(rotgroup);
	layout->addWidget(scgroup);
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
	Vector3f pos = m_transform.getPosition();
	m_x->setText(QString().setNum(pos.x,'g',3));
	m_y->setText(QString().setNum(pos.y,'g',3));
	m_z->setText(QString().setNum(pos.z,'g',3));

	Vector3f rot = m_transform.toEuler();
	m_rx->setText(QString().setNum(rot.x,'g',3));
	m_ry->setText(QString().setNum(rot.y,'g',3));
	m_rz->setText(QString().setNum(rot.z,'g',3));
}
