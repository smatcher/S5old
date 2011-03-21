#ifndef NODEWIDGET_H
#define NODEWIDGET_H

#include <QWidget>

#include "core/scenegraph/node.h"
#include "debug/widgets/transformwidget.h"
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
	virtual ~NodeWidget();

	virtual void updateData();
};

#endif // NODEWIDGET_H
