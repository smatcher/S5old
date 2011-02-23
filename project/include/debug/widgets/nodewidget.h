#ifndef NODEWIDGET_H
#define NODEWIDGET_H

#include <QWidget>

#include "include/core/scenegraph/node.h"
#include "include/debug/widgets/transformwidget.h"
#include <QLayout>

class NodeWidget : public QWidget
{
	Q_OBJECT

private :
	Node& m_node;
	QLayout* m_layout;
	TransformWidget* m_transformWidget;

public :
	NodeWidget(Node& node);
	~NodeWidget();
};

#endif // NODEWIDGET_H
