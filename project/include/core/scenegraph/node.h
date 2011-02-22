#ifndef NODE_H
#define NODE_H

#include "include/core/utils/parenting.h"
#include "include/core/maths/transform.h"

class Node : public ParentOf<Node>, public ChildOf< ParentOf<Node> >, public Transform<double>
{
public:
	Node(const QString& name = "Anon Node");
};

#endif // NODE_H
