#ifndef NODE_H
#define NODE_H

#include "core/utils/parenting.h"
#include "core/maths/transform.h"
#include "core/properties/propertyset.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/parentofnode.h"

class GLWidget;

#ifdef WITH_TOOLS
	class NodeWidget;
#endif

class Node : public ParentOfNode, public ChildOf< ParentOfNode >, public Transform<double>
{
	#ifdef WITH_TOOLS
		friend class NodeWidget;
	#endif

private :
	PropertySet m_properties;

	#ifdef WITH_TOOLS
		NodeWidget* m_widget;
		void widgetDestroyed();
	#endif

public:
	Node(const QString& name = "Anon Node");
	virtual ~Node();
	virtual ParentOfNode::TYPE type() {return ParentOfNode::NODE;}

	PropertySet& properties();
    const PropertySet& properties() const;

	Transformd globalTransform();

	void addProperty(IProperty* property);

	// Debug
	virtual void drawDebug(const GLWidget* widget, bool recursive) const;

	#ifdef WITH_TOOLS
		virtual NodeWidget* getWidget();
	#endif
};

#endif // NODE_H
