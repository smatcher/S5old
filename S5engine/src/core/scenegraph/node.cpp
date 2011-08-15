#include "core/scenegraph/node.h"
#include "QtOpenGL"
#include <QColor>

#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/nodewidget.h"
#endif

Node::Node(const QString& name) : ChildOf<ParentOfNode>(name), Transform<float>(), m_properties(this)
{
	#ifdef WITH_TOOLS
		m_widget = NULL;
	#endif
}

Node::~Node()
{

}

Matrix4f Node::getGlobalTransform(bool with_scale) const
{
	Matrix4f ret;

	Matrix4f self_mat;

	if(with_scale) {
		const Transformf* trans = static_cast<const Transformf*>(this);
		self_mat = Matrix4f(*trans);
	} else {
		self_mat = Matrix4f(rotation,position);
	}

	if(parent() != NULL)
	{
		if(parent()->type() == ParentOfNode::NODE)
		{
			const Node* parentNode = static_cast<const Node*>(parent());
			ret = parentNode->getGlobalTransform(with_scale) * self_mat;
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

void Node::setGlobalTransform(Transformf transform)
{
	if(parent() != NULL && parent()->type() == ParentOfNode::NODE)
	{
		Node* parentNode = static_cast<Node*>(parent());
		Transformf parent_trans(parentNode->getGlobalTransform(false));
		parent_trans.invert();
		transform *= parent_trans;
		rotation = transform.getRotation();
		position = transform.getPosition();
	}
	else
	{
		rotation = transform.getRotation();
		position = transform.getPosition();
	}
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

Node* Node::find(QString name)
{
	Node* ret = child(name);

	if(ret == NULL) {
		for(int i=0 ; i< childCount() ; i++) {
			ret = child(i)->find(name);
			if(ret != NULL)
				break;
		}
	}

	return ret;
}

PropertySet& Node::properties()
{
	return m_properties;
}

const PropertySet& Node::properties() const
{
	return m_properties;
}

void Node::onLinked(Node * to)
{
	#ifdef WITH_TOOLS
	#endif
}

void Node::onUnlinked(Node * from)
{
	#ifdef WITH_TOOLS
	#endif
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
