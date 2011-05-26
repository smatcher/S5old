#ifndef PARENTOFNODE_H
#define PARENTOFNODE_H

#include "core/utils/parenting.h"

class Node;
class ParentOfNode : public ParentOf<Node>
{
public :

	enum TYPE
	{
		NODE,
		SCENEGRAPH
	};

	virtual TYPE type() = 0;
};

#endif // PARENTOFNODE_H
