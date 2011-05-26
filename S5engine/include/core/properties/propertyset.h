#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include "iproperty.h"

class IProperty;
class Node;

class PropertySet : public ParentOf<IProperty>
{
private :
	Node* m_node;

public:
	PropertySet(Node* m_node);
	virtual ~PropertySet();

	Node* node();
};

#endif // PROPERTYSET_H
