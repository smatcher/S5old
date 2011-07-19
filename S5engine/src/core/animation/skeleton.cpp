#include <core/animation/skeleton.h>

Node* Skeleton::Bone::buildNodes(bool isRoot)
{
	Node* ret;
	if(isRoot) {
		ret = new Node(m_name);
	} else {
		ret = new BoneNode(m_name);
	}

	ret->copy(m_bind_pose);

	for(QVector<Bone>::iterator it = m_childrens.begin() ; it != m_childrens.end() ; it++) {
		ret->link((*it).buildNodes(false));
	}

	return ret;
}

Node* Skeleton::buildSkeleton()
{
	return m_root_bone.buildNodes(true);
}

void BoneNode::drawDebug(const GLWidget* widget, bool recursive) const
{
	glPushMatrix();
	glDisable(GL_DEPTH_TEST);
		this->glMultd();
		widget->qglColor(Qt::white);
		for(int i=0 ; i<childCount() ; i++) {
			glBegin(GL_LINES);
				glVertex3d(0,0,0);
				Node* son = child(i);
				Vector3f position = son->getPosition();
				glVertex3d(position.x,position.y,position.z);
			glEnd();
		}
	glEnable(GL_DEPTH_TEST);

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
