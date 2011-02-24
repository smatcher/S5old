#include "include/core/properties/dummyupdatable.h"
#include "include/core/scenegraph/node.h"

DummyUpdatable::DummyUpdatable() : IUpdatable(QString("DummyUpdatable"))
{
    this->angle = 0.1;
}

void DummyUpdatable::update(double elapsed_time)
{
    Node* parentNode = (Node*)this->parent();
    parentNode->rotate(Vector3d(1.0, 0.0, 0.0), angle*elapsed_time);
}
