#include "core/scenegraph/node.h"
#include "QtOpenGL"
#include <QColor>

#include "core/utils/customevents.h"

#include "core/log/log.h"

#ifdef WITH_TOOLS
	#include "tools/mvc/scenegraphmodel.h"
	#include "tools/widgets/nodewidget.h"
#endif

Node::Node(const QString& name) : ChildOf<ParentOfNode>(name), Transform<float>(), m_scene(0), m_properties(this)
{
	#ifdef WITH_TOOLS
		m_widget = 0;
		m_selected = false;
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

	if(parent() != 0)
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
	if(parent() != 0 && parent()->type() == ParentOfNode::NODE)
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

void Node::drawDebug(const GLWidget* widget, const RenderManager::DebugGizmosFilter& filter, bool recursive) const
{
	glPushMatrix();
		this->glMultd();

		if(filter.draw_transforms)
		{
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
		}

		for(int i=0 ; i<properties().childCount() ; i++)
		{
			properties().child(i)->drawDebug(widget,filter);
		}

		if(recursive)
		{
			for(int i= 0 ; i<childCount() ; i++)
			{
				child(i)->drawDebug(widget,filter, true);
			}
		}
	glPopMatrix();
}

Node* Node::find(QString name)
{
	Node* ret = child(name);

	if(ret == 0) {
		for(int i=0 ; i< childCount() ; i++) {
			ret = child(i)->find(name);
			if(ret != 0)
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

void Node::changedScenegraph(SceneGraph* scene)
{
	if(scene != m_scene) {
		m_scene = scene;

		for(int i=0 ; i < childCount() ; i++) {
			child(i)->changedScenegraph(scene);
		}
	}
}

void Node::onLinked(ParentOfNode * to)
{
	SceneGraph* scene;

	if(to->type() == ParentOfNode::SCENEGRAPH) {
		scene = static_cast<SceneGraph*>(to);
	} else {
		scene = static_cast<Node*>(to)->m_scene;
	}

	changedScenegraph(scene);

	#ifdef WITH_TOOLS
		if(m_scene != 0) {
			QCoreApplication::postEvent(m_scene->getDebugModel(),new UPDATED_EVENT());
		}
	#endif
}

void Node::onUnlinked(ParentOfNode * from)
{
	#ifdef WITH_TOOLS
		if(m_scene != 0) {
			QCoreApplication::postEvent(m_scene->getDebugModel(),new UPDATED_EVENT());
		}
	#endif

	changedScenegraph(0);
}

#ifdef WITH_TOOLS

	void Node::setSelected(bool selected)
	{
		m_selected = selected;
	}

	bool Node::isSelected()
	{
		return m_selected;
	}

	NodeWidget* Node::getWidget()
	{
		if(m_widget == 0)
			m_widget = new NodeWidget(*this);

		return m_widget;
	}

	void Node::widgetDestroyed()
	{
		m_widget = 0;
	}

#endif
