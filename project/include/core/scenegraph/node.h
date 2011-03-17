#ifndef NODE_H
#define NODE_H

#include "core/utils/parenting.h"
#include "core/maths/transform.h"
#include "core/properties/propertyset.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/parentofnode.h"

class GLWidget;
class NodeWidget;

class Node : public ParentOfNode, public ChildOf< ParentOfNode >, public Transform<double>
{
	friend class NodeWidget;
private :
	PropertySet m_properties;
	NodeWidget* m_widget;

	void widgetDestroyed();
public:
	Node(const QString& name = "Anon Node");
	virtual ~Node();
	virtual ParentOfNode::TYPE type() {return ParentOfNode::NODE;}

	PropertySet& properties();
    const PropertySet& properties() const;

	Transformd globalTransform();

	// Debug
    virtual void drawDebug(const GLWidget* widget, bool recursive) const;
	virtual NodeWidget* getWidget();
};

#endif // NODE_H
