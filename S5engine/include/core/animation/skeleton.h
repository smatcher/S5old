#ifndef SKELETON_H
#define SKELETON_H

#include <core/maths/transform.h>
#include <core/maths/matrix4.h>
#include <core/scenegraph/node.h>
#include <QVector>
#include <QString>

#include "core/animation/animation.h"

class AssimpFactory;

class BoneNode : public Node
{
public:
	BoneNode(const QString& name) : Node(name) {}
	virtual void drawDebug(const GLWidget *widget, const RenderManager::DebugGizmosFilter& filter, bool recursive) const;
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
		Matrix4f m_inverted_global_pose;

		Node* buildNodes(bool isRoot);
		void computeInversedGlobalPose(Matrix4f parent_transform);
	};

	Node* buildSkeleton();
	Skeleton::Bone* getRootBone();
	Animation* getAnimation();

protected:

	Bone m_root_bone;
	QVector<Animation> TMP_animations;

	void computeInversedGlobalPoses();
};

#endif //SKELETON_H
