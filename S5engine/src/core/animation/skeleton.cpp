#include <core/animation/skeleton.h>
#include <core/maths/trigo.h>
#include <QColor>

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

void Skeleton::Bone::computeInversedGlobalPose(Matrix4f parent_transform)
{
	Matrix4f global_transform = parent_transform * (Matrix4f)m_bind_pose;
	m_inverted_global_pose = global_transform.getInverse();

	for(QVector<Bone>::iterator it = m_childrens.begin() ; it != m_childrens.end() ; it++) {
		it->computeInversedGlobalPose(global_transform);
	}
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
/*
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
*/

		QColor color = Qt::white;
		color.setAlpha(200);
		widget->qglColor(color);
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

void Skeleton::computeInversedGlobalPoses()
{
	m_root_bone.computeInversedGlobalPose(Matrix4f());
}

Skeleton::Bone* Skeleton::getRootBone()
{
	return &m_root_bone;
}

Animation* Skeleton::getAnimation()
{
	return &TMP_animations[0];
}

