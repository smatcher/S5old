#include "core/properties/iproperty.h"
#include <QGroupBox>

IProperty::IProperty(const QString& propName) : ChildOf<PropertySet>(propName)
{
	#ifdef WITH_TOOLS
		m_widget = 0;
	#endif
}

IProperty::~IProperty()
{

}

Node* IProperty::node()
{
	PropertySet* _parent = parent();
	if(_parent != 0)
		return parent()->node();
	else
		return 0;
}

const Node* IProperty::node() const
{
	const PropertySet* _parent = parent();
	if(_parent != 0)
		return parent()->node();
	else
		return 0;
}

void IProperty::drawDebug(const GLWidget* widget, const RenderManager::DebugGizmosFilter& filter) const
{
}

#ifdef WITH_TOOLS

PropertyWidget* IProperty::getWidget()
{
	if(m_widget == 0)
	{
		m_widget = new PropertyWidget(this);
	}

	return m_widget;
}

void IProperty::widgetDestroyed()
{
	m_widget = 0;
}

#endif
