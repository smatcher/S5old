#include "core/animation/skeletonanimator.h"
#include "core/scenegraph/node.h"

#include "core/log/log.h"

SkeletonAnimator::SkeletonAnimator(Skeleton* skeleton) : IUpdatable("SkeletonAnimator")
{
	m_animationtime = 0.0;
	m_skeleton = skeleton;
}

void SkeletonAnimator::update(double elapsed)
{
	m_animationtime += 10*elapsed;

	Animation* anim = m_skeleton->getAnimation();
	while(m_animationtime > anim->m_duration) {
		m_animationtime -= anim->m_duration;

		debug("ANIMATION", "loop");
	}

	//debug("ANIMATION", "time" << m_animationtime);
	for(int i=0 ; i<anim->m_channels.count() ; i++) {
		QMap<QString, QPair<Skeleton::Bone*, Node*> >::iterator link;
		link = m_links.find(anim->m_channels[i].m_name);
		//Skeleton::Bone* bone = link.value().first;
		Node* node = link.value().second;

		int j=0;
		while(
			(j+1)<anim->m_channels[i].m_rotation_keys.count() &&
			m_animationtime > anim->m_channels[i].m_rotation_keys[j].time
			) {
			j++;
		}
		//if(i == 3) {debug("ANIMATION","rotation " << j);}
		Matrix3f rot = anim->m_channels[i].m_rotation_keys[j].value;
		j=0;
		while(
			(j+1)<anim->m_channels[i].m_scaling_keys.count() &&
			m_animationtime > anim->m_channels[i].m_scaling_keys[j].time
			) {
			j++;
		}
		Vector3f scale = anim->m_channels[i].m_scaling_keys[j].value;
		//if(i == 3) {debug("ANIMATION","scale " << j << scale);}
		j=0;
		while(
			(j+1)<anim->m_channels[i].m_translation_keys.count() &&
			m_animationtime > anim->m_channels[i].m_translation_keys[j].time
			) {
			j++;
		}
		Vector3f pos = anim->m_channels[i].m_translation_keys[j].value;
		//if(i == 3) {debug("ANIMATION","pos " << j << pos);}

		Transformf trans(rot,pos,scale);
		node->copy(trans);
	}
}

void SkeletonAnimator::createLinks()
{
	linkBoneToNode(m_skeleton->getRootBone(), node());
}

void SkeletonAnimator::linkBoneToNode(Skeleton::Bone* bone, Node* node)
{
	for(int i=0 ; i<node->childCount() ; i++) {
		Node* child = node->child(i);

		if(child->getName() == bone->m_name) {
			QPair<Skeleton::Bone*, Node*> pair(bone, child);
			m_links.insert(child->getName(), pair);

			for(int j=0 ; j<bone->m_childrens.count() ; j++) {
				linkBoneToNode(&bone->m_childrens[j], child);
			}
		}
	}
}
