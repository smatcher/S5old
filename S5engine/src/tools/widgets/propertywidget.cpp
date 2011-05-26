#include "tools/widgets/propertywidget.h"
#include "core/properties/iproperty.h"
#include <QHBoxLayout>
#include <QGroupBox>

PropertyWidget::PropertyWidget(IProperty *property)
{
	m_property = property;
	QHBoxLayout* layout = new QHBoxLayout();
	layout->setMargin(0);
	m_box = new QGroupBox(property->getName());
	layout->addWidget(m_box);
	setLayout(layout);
	setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Maximum);
}

PropertyWidget::~PropertyWidget()
{
	m_property->widgetDestroyed();
}

void PropertyWidget::updateData() {}
