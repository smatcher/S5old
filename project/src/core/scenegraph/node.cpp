#include "include/core/scenegraph/node.h"

Node::Node(const QString& name) : ParentOf<Node>(), ChildOf< ParentOf<Node> >(name)
{
}
