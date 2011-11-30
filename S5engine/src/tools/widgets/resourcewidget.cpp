#include "tools/widgets/resourcewidget.h"
#include "core/resources/resource.h"

#include <QVBoxLayout>
#include <QLabel>

ResourceWidget::ResourceWidget(ResourceData &resource) : QGroupBox(resource.name()), m_resource(resource)
{
	m_layout = new QVBoxLayout();

	QLabel* label = new QLabel(QString("Status : ")+((m_resource.m_state == ResourceData::STATE_LOADED)?"Loaded":"Not Loaded"));
	m_layout->addWidget(label);
	m_layout->addStretch();

	setMinimumWidth(240);
	setLayout(m_layout);
}

ResourceWidget::~ResourceWidget()
{
	m_resource.widgetDestroyed();
}

void ResourceWidget::updateData()
{
}
