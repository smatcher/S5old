#ifndef IPROPERTY_H
#define IPROPERTY_H

#include "core/utils/parenting.h"
#include "core/properties/propertyset.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/propertywidget.h"
#endif

class GLWidget;
class PropertySet;
class Node;

class IProperty : public ChildOf<PropertySet>
{

public:
   /// Constructor
	IProperty(const QString& name = "Basic Property");
	/// Destructor
	virtual ~IProperty();

	Node* node();

	// Debug
	virtual void drawDebug(const GLWidget* widget) const;

#ifdef WITH_TOOLS

	friend class PropertyWidget;

	protected:
		PropertyWidget* m_widget;

	private:
		void widgetDestroyed();

	public:
		virtual PropertyWidget* getWidget();

#endif

};

#endif // IPROPERTY_H
