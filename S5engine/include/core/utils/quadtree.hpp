// This include should fail since quadtree.h includes quadtree.hpp
// However QtCreator needs this include in order to provide code assistance.
#include "quadtree.h"

template<class T>
QuadTree<T>::QuadTree(T* value, QuadNode* parent = 0, POSITION position = ROOT) :
m_value(value), m_parent(parent), m_position(position) {

	m_children[0] = 0;
	m_children[1] = 0;
	m_children[2] = 0;
	m_children[3] = 0;

}

template<class T>
QuadTree<T>::~QuadTree() {

	delete m_children[0];
	delete m_children[1];
	delete m_children[2];
	delete m_children[3];

}

template<class T>
QuadTree<T>* QuadTree<T>::parent() {
	return m_parent;
}

template<class T>
QuadTree<T>* QuadTree<T>::child(POSITION position) {
	return m_children[position];
}

template<class T>
void QuadTree<T>::addChild(POSITION position, T* value) {
	m_children[position] = new QuadNode(value, this, position);
}

template<class T>
bool QuadTree<T>::isRoot() {
	return (m_parent == 0);
}

template<class T>
bool QuadTree<T>::isLeaf() {
	return (m_children[0]==0)&&(m_children[1]==0)&&(m_children[2]==0)&&(m_children[3]==0);
}

template<class T>
T* QuadTree<T>::getValue() {
	return m_value;
}
