#ifndef NODE_H
#define NODE_H

#include "include/core/utils/parenting.h"
#include "include/core/maths/transform.h"
#include "include/core/properties/propertyset.h"
#include "include/glwidget.h"

class GLWidget;
class NodeWidget;

class Node : public ParentOf<Node>, public ChildOf< ParentOf<Node> >, public Transform<double>
{
	friend class NodeWidget;
private :
	PropertySet m_properties;
	NodeWidget* m_widget;

	void widgetDestroyed();
public:
	Node(const QString& name = "Anon Node");
	virtual ~Node();

	PropertySet& properties();

	void drawTransform(const GLWidget* widget, bool recursive) const;
	virtual NodeWidget* getWidget();
};

#endif // NODE_H
