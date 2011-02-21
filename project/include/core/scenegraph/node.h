#ifndef NODE_H
#define NODE_H

#include "include/core/utils/parenting.h"

class Node : public ParentOf<Node>, public ChildOf< ParentOf<Node> >
{
public:
	Node(const QString& name = "Anon Node");
};

#endif // NODE_H
