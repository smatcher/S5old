#ifndef SKELETONANIMATOR_H
#define SKELETONANIMATOR_H

#include "core/properties/iupdatable.h"
#include "core/animation/skeleton.h"

#include <QMap>
#include <QPair>

class SkeletonAnimator : public IUpdatable
{
public:
	SkeletonAnimator(Skeleton* skeleton);

	void update(double elapsed);
	void createLinks();

private:
	double m_animationtime;
	Skeleton* m_skeleton;
	QMap<QString, QPair<Skeleton::Bone*, Node*> > m_links;

	void linkBoneToNode(Skeleton::Bone* bone, Node* node);
};

#endif // SKELETONANIMATOR_H
