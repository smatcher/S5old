#include <QApplication>
#include <QDesktopWidget>

#include "include/window.h"
#include "include/core/scenegraph/scenegraph.h"
#include "include/core/scenegraph/node.h"

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	SceneGraph sg;
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

	sg.link(&n1);
	sg.link(&n2);
	sg.link(&n3);
	sg.link(&n4);
	n1.link(&s11);
	n1.link(&s12);
	n2.link(&s21);
	s21.link(&s211);
	n4.link(&s41);
	s41.link(&nn);
	s41.link(&pro);

	Window window(sg);
	window.resize(window.sizeHint());
	int desktopArea = QApplication::desktop()->width() *
					  QApplication::desktop()->height();
	int widgetArea = window.width() * window.height();

	if (((float)widgetArea / (float)desktopArea) < 0.75f)
		window.show();
	else
		window.showMaximized();

	int ret = app.exec();

	sg.unlinkAll();
	n1.unlinkAll();
	n2.unlinkAll();
	n3.unlinkAll();
	n4.unlinkAll();

	return ret;
}
