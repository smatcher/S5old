#include "core/scenegraph/node.h"
#include "debug/widgets/nodewidget.h"
#include "QtOpenGL"
#include <QColor>

Node::Node(const QString& name) : ChildOf<ParentOfNode>(name), Transform<double>(), m_properties(this)
{
	m_widget = NULL;
}

Node::~Node()
{

}

Transformd Node::globalTransform()
{
	Transformd ret;

	Transformd* trans = static_cast<Transformd*>(this);

	if(parent()->type() == ParentOfNode::NODE)
	{
		Node* parentNode = static_cast<Node*>(parent());
		ret = parentNode->globalTransform() * (*trans);
	}
	else
	{
		ret = (*trans);
	}

	return ret;
}

void Node::drawDebug(const GLWidget* widget, bool recursive) const
{
	glPushMatrix();
		this->glMultd();

		widget->qglColor(Qt::red);
		glBegin(GL_LINES);
			glVertex3d(0,0,0);
			glVertex3d(1,0,0);
		glEnd();
		widget->qglColor(Qt::green);
		glBegin(GL_LINES);
			glVertex3d(0,0,0);
			glVertex3d(0,1,0);
		glEnd();
		widget->qglColor(Qt::blue);
		glBegin(GL_LINES);
			glVertex3d(0,0,0);
			glVertex3d(0,0,1);
		glEnd();

        for(int i=0 ; i<properties().childCount() ; i++)
        {
            properties().child(i)->drawDebug(widget);
        }

		if(recursive)
		{
			for(int i= 0 ; i<childCount() ; i++)
			{
                child(i)->drawDebug(widget,true);
			}
		}
	glPopMatrix();
}

PropertySet& Node::properties()
{
	return m_properties;
}

const PropertySet& Node::properties() const
{
    return m_properties;
}

NodeWidget* Node::getWidget()
{
	if(m_widget == NULL)
		m_widget = new NodeWidget(*this);

	return m_widget;
}

void Node::widgetDestroyed()
{
	m_widget = NULL;
}
