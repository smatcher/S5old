#ifndef SKELETON_H
#define SKELETON_H

#include <core/maths/transform.h>
#include <core/scenegraph/node.h>
#include <QVector>
#include <QString>

class AssimpFactory;

class BoneNode : public Node
{
public:
	BoneNode(const QString& name) : Node(name) {}
	virtual void drawDebug(const GLWidget *widget, bool recursive) const;
};

class Skeleton
{
	friend class AssimpFactory;

public:

	struct Bone
	{
		QString m_name;
		QVector<Bone> m_childrens;
		Transformf m_bind_pose;

		Node* buildNodes(bool isRoot);
	};

	Node* buildSkeleton();

protected:

	Bone m_root_bone;
};

#endif //SKELETON_H
