#include "include/core/properties/propertyset.h"

PropertySet::PropertySet(Node* node)
{
	m_node = node;
}

PropertySet::~PropertySet()
{
}


Node* PropertySet::node()
{
	return m_node;
}
