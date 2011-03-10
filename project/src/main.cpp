#include <QApplication>

#include "core/framework/engine.h"
#include "core/scenegraph/scenegraph.h"
#include "core/scenegraph/node.h"
#include "core/maths/trigo.h"
#include "core/properties/dummyupdatable.h"

int main(int argc, char *argv[])
{
	Engine engine(argc, argv);

	SceneGraph* sg = engine.getScenegraph_TEMPORARY();
	Node n1("Node 1");
	Node s11("Son 1 of 1");
	Node s12("Son 2 of 1");
	Node n2("Node 2");
	Node s21("Son 1 of 2");
	Node s211("Son 1 of son 1 of 2 (yeah right)");
	Node n3("Node 3");
	Node n4("Node 4");
	Node s41("Son 1 of 4");
	Node nn("????");
        Node pro("Profit");

        IProperty prop1;
        DummyUpdatable prop2;

        n1.properties().link(&prop1);
        n1.properties().link(&prop2);

	sg->link(&n1);
	sg->link(&n2);
	sg->link(&n3);
	sg->link(&n4);
	n1.link(&s11);
	n1.link(&s12);
	n2.link(&s21);
	s21.link(&s211);
	n4.link(&s41);
	s41.link(&nn);
	s41.link(&pro);

	n1.moveTo(Vector3d(2,2,2));
	n1.rotate(Vector3d(0,1,0),75);
	s11.moveTo(Vector3d(0.5,0,0.5));
	s12.moveTo(Vector3d(-0.5,0,-0.5));

	int ret = engine.start();

	n1.unlinkAll();
	n2.unlinkAll();
	n3.unlinkAll();
	n4.unlinkAll();

	return ret;
}
