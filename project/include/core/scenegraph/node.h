#ifndef NODE_H
#define NODE_H

#include "include/core/utils/parenting.h"
#include "include/core/maths/transform.h"
#include "include/glwidget.h"

class GLWidget;

class Node : public ParentOf<Node>, public ChildOf< ParentOf<Node> >, public Transform<double>
{
public:
	Node(const QString& name = "Anon Node");
	void drawTransform(const GLWidget* widget, bool recursive) const;
};

#endif // NODE_H
