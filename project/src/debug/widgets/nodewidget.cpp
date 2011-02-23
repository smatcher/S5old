#include "include/debug/widgets/nodewidget.h"

NodeWidget::NodeWidget(Node& node) : m_node(node)
{
	m_layout = new QVBoxLayout();

	m_transformWidget = new TransformWidget(node);

	m_layout->addWidget(m_transformWidget);
	setLayout(m_layout);
}

NodeWidget::~NodeWidget()
{
	m_node.widgetDestroyed();
}
