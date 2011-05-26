#include "core/properties/iproperty.h"
#include <QGroupBox>

IProperty::IProperty(const QString& propName) : ChildOf<PropertySet>(propName)
{
	#ifdef WITH_TOOLS
		m_widget = NULL;
	#endif
}

IProperty::~IProperty()
{

}

Node* IProperty::node()
{
    PropertySet* _parent = parent();
    if(_parent != NULL)
        return parent()->node();
    else
        return NULL;
}

void IProperty::drawDebug(const GLWidget*) const
{
}

#ifdef WITH_TOOLS

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

#endif
