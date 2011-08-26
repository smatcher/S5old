#ifndef MANIPULATOR_H
#define MANIPULATOR_H

#include <QVector>

class GLWidget;
class Node;

class Manipulator
{
private :
	QVector<Node*> m_selected;

public :
	void draw(GLWidget* widget);
	void update();

	void addSelected(Node* node);
	void removeSelected(Node* node);
	void unselectAll();
	void select(Node* node);
};

#endif // MANIPULATOR_H
