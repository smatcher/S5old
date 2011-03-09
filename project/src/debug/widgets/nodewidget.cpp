#include "debug/widgets/nodewidget.h"
#include <QSizePolicy>

NodeWidget::NodeWidget(Node& node) : m_node(node)
{
	setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
	m_layout = new QVBoxLayout();

	m_transformWidget = new TransformWidget(node);
	m_layout->addWidget(m_transformWidget);

	for(int i=0 ; i<node.properties().childCount() ; i++)
	{
		QWidget* widget = node.properties().child(i)->getWidget();
		m_layout->addWidget(widget);
	}

	setLayout(m_layout);
}

NodeWidget::~NodeWidget()
{
	m_node.widgetDestroyed();
}
