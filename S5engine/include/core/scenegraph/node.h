#ifndef NODE_H
#define NODE_H

#include "core/utils/parenting.h"
#include "core/maths/transform.h"
#include "core/maths/boundingvolume.h"
#include "core/properties/propertyset.h"
#include "core/framework/glwidget.h"
#include "core/scenegraph/parentofnode.h"

class GLWidget;

#ifdef WITH_TOOLS
	class NodeWidget;
#endif

class Node : public ParentOfNode, public ChildOf< ParentOfNode >, public Transform<float>
{
	#ifdef WITH_TOOLS
		friend class NodeWidget;
	#endif

private :
	PropertySet m_properties;
	BoundingVolume* m_bounding;

	#ifdef WITH_TOOLS
		NodeWidget* m_widget;
		void widgetDestroyed();
	#endif

public:
	Node(const QString& name = "Anon Node");
	virtual ~Node();
	virtual ParentOfNode::TYPE type() const {return ParentOfNode::NODE;}

	PropertySet& properties();
	const PropertySet& properties() const;

	Matrix4f getGlobalTransform(bool with_scale=true) const;
	void setGlobalTransform(Transformf transform);

	void addProperty(IProperty* property);

	/// Recursivly find the first child node with this name
	Node* find(QString name);

	virtual void onLinked(Node * to);
	virtual void onUnlinked(Node * from);

	// Debug
	virtual void drawDebug(const GLWidget* widget, bool recursive) const;

	#ifdef WITH_TOOLS
		virtual NodeWidget* getWidget();
	#endif
};

#endif // NODE_H
