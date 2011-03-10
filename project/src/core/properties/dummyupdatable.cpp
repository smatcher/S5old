#include "core/properties/dummyupdatable.h"
#include "core/scenegraph/node.h"

DummyUpdatable::DummyUpdatable() : IUpdatable(QString("DummyUpdatable"))
{
	this->angle = 180;
}

void DummyUpdatable::update(double elapsed_time)
{
    std::cout << "Updating DummyUpdatable" << std::endl;
	Node* parentNode = this->node();
	parentNode->rotate(Vector3d(0.0, 1.0, 0.0), angle*elapsed_time);
}
