#include "include/core/scenegraph/node.h"
#include "QtOpenGL"
#include <QColor>

Node::Node(const QString& name) : ParentOf<Node>(), ChildOf< ParentOf<Node> >(name), Transform<double>()
{
}

void Node::drawTransform(const GLWidget* widget, bool recursive) const
{
	glPushMatrix();
		this->glMultd();

		widget->qglColor(Qt::red);
		glBegin(GL_LINE);
			glVertex3d(0,0,0);
			glVertex3d(1,0,0);
		glEnd();
		widget->qglColor(Qt::green);
		glBegin(GL_LINE);
			glVertex3d(0,0,0);
			glVertex3d(0,1,0);
		glEnd();
		widget->qglColor(Qt::blue);
		glBegin(GL_LINE);
			glVertex3d(0,0,0);
			glVertex3d(0,0,1);
		glEnd();

		if(recursive)
		{
			for(int i= 0 ; i<childCount() ; i++)
			{
				child(i)->drawTransform(widget,true);
			}
		}
	glPopMatrix();
}
