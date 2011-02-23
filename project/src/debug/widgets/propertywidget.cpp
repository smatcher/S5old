#include "include/debug/widgets/propertywidget.h"
#include "include/core/properties/iproperty.h"
#include <QHBoxLayout>
#include <QGroupBox>

PropertyWidget::PropertyWidget(IProperty *property)
{
	m_property = property;
	QHBoxLayout* layout = new QHBoxLayout();
	QGroupBox* box = new QGroupBox(property->getName());
	layout->addWidget(box);
	setLayout(layout);
}

PropertyWidget::~PropertyWidget()
{
	m_property->widgetDestroyed();
}
