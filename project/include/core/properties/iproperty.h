#ifndef IPROPERTY_H
#define IPROPERTY_H

#include "include/core/utils/parenting.h"
#include "include/core/properties/propertyset.h"
#include "include/debug/widgets/propertywidget.h"

class PropertySet;

class IProperty : public ChildOf<PropertySet>
{
	friend class PropertyWidget;
private:
	PropertyWidget* m_widget;

	void widgetDestroyed();
public:
    IProperty();
	~IProperty();

	virtual PropertyWidget* getWidget();
};

#endif // IPROPERTY_H
