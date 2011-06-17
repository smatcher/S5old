#ifndef QUADTREE_H
#define QUADTREE_H

template<class T>
class QuadTree
{
public:
	enum POSITION{
		SOUTH_EAST,
		SOUTH_WEST,
		NORTH_EAST,
		NORTH_WEST,
		ROOT
	};
	typedef QuadTree<T> QuadNode;

	QuadTree(T* value, QuadNode* parent, POSITION position);
	~QuadTree();

	QuadNode* parent();
	QuadNode* child(POSITION position);

	void addChild(POSITION position, T* value);

	bool isRoot();
	bool isLeaf();

	T* getValue();

private:
	T* m_value;

	POSITION m_position;
	QuadNode* m_parent;
	QuadNode* m_children[4];

};

#include "quadtree.hpp"

#endif // QUADTREE_H
