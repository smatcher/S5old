#include "tools/widgets/resourcewidget.h"
#include "core/resources/resource.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

ResourceWidget::ResourceWidget(ResourceData &resource) : QGroupBox(resource.name()), m_resource(resource)
{
	m_layout = new QVBoxLayout();

	m_status = new QLabel(QString("Status : ")+((m_resource.m_state == ResourceData::STATE_LOADED)?"Loaded":"Not Loaded"));
	m_layout->addWidget(m_status);
	m_reloadButton = new QPushButton((m_resource.m_state == ResourceData::STATE_LOADED)?"Reload":"Load");
	m_layout->addWidget(m_reloadButton);
	m_layout->addStretch();

	setMinimumWidth(240);
	setLayout(m_layout);

	connect(m_reloadButton,SIGNAL(clicked()),this,SLOT(reloadResource()));
}

ResourceWidget::~ResourceWidget()
{
	m_resource.widgetDestroyed();
}

void ResourceWidget::updateData()
{
	m_status->setText(QString("Status : ")+((m_resource.m_state == ResourceData::STATE_LOADED)?"Loaded":"Not Loaded"));
	m_reloadButton->setText((m_resource.m_state == ResourceData::STATE_LOADED)?"Reload":"Load");
}

void ResourceWidget::reloadResource()
{
	if(m_resource.m_state == ResourceData::STATE_UNLOADED)
		m_resource.load();
	else
		m_resource.reload();

	updateData();
}
