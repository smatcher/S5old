#include "core/scenegraph/node.h"
#include "QtOpenGL"
#include <QColor>

#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/nodewidget.h"
#endif

Node::Node(const QString& name) : ChildOf<ParentOfNode>(name), Transform<double>(), m_properties(this)
{
	#ifdef WITH_TOOLS
		m_widget = NULL;
	#endif
}

Node::~Node()
{

}

Matrix4d Node::globalTransform(bool with_scale)
{
	Matrix4d ret;

	Matrix4d self_mat;

	if(with_scale) {
		Transformd* trans = static_cast<Transformd*>(this);
		self_mat = Matrix4d(*trans);
	} else {
		self_mat = Matrix4d(rotation,position);
	}

	if(parent() != NULL)
	{
		if(parent()->type() == ParentOfNode::NODE)
		{
			Node* parentNode = static_cast<Node*>(parent());
			ret = parentNode->globalTransform(with_scale) * self_mat;
		}
		else
		{
			ret = self_mat;
		}
	}
	else
	{
		logWarn("trying to access globalTransform on unlinked node");
	}
	return ret;
}

void Node::addProperty(IProperty *property)
{
	m_properties.link(property);
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

#ifdef WITH_TOOLS

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

#endif
