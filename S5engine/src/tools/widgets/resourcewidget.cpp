#include "tools/widgets/resourcewidget.h"
#include "core/resources/resource.h"

#include <QVBoxLayout>
#include <QLabel>

ResourceWidget::ResourceWidget(ResourceData &resource) : QWidget(), m_resource(resource)
{
	QVBoxLayout* layout = new QVBoxLayout();

	QLabel* label = new QLabel(m_resource.name());
	label->setFrameStyle(QFrame::StyledPanel | QFrame::Raised);
	layout->addWidget(label);
	label = new QLabel((m_resource.m_state == ResourceData::STATE_LOADED)?"Loaded":"Not Loaded");
	layout->addWidget(label);
	layout->addStretch();

	setMinimumWidth(240);
	setLayout(layout);
}

ResourceWidget::~ResourceWidget()
{
	m_resource.widgetDestroyed();
}

void ResourceWidget::updateData()
{
}
