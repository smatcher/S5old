#ifndef IPROPERTY_H
#define IPROPERTY_H

#include "core/utils/parenting.h"
#include "core/properties/propertyset.h"
#include "debug/widgets/propertywidget.h"

class GLWidget;
class PropertySet;
class Node;

class IProperty : public ChildOf<PropertySet>
{
	friend class PropertyWidget;
private:
	PropertyWidget* m_widget;

	void widgetDestroyed();
public:
   /// Constructor
	IProperty(const QString& name = "Basic Property");
	/// Destructor
	virtual ~IProperty();

	Node* node();

    // Debug
    virtual void drawDebug(const GLWidget* widget) const;
    virtual PropertyWidget* getWidget();
};

#endif // IPROPERTY_H
