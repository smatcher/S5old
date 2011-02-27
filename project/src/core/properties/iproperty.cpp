#include "include/core/properties/iproperty.h"
#include <QGroupBox>

IProperty::IProperty(const QString& propName) : ChildOf<PropertySet>(propName)
{
	m_widget = NULL;
}

IProperty::~IProperty()
{

}

Node* IProperty::node()
{
	return parent()->node();
}

PropertyWidget* IProperty::getWidget()
{
	if(m_widget == NULL)
	{
		m_widget = new PropertyWidget(this);
	}

	return m_widget;
}

void IProperty::widgetDestroyed()
{
	m_widget = NULL;
}

