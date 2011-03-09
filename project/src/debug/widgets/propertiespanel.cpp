#include "debug/widgets/propertiespanel.h"
#include <QHBoxLayout>

PropertiesPanel::PropertiesPanel()
{
	m_layout = new QHBoxLayout();
	m_layout->setMargin(0);
	m_widget = NULL;

	setLayout(m_layout);

	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
}

PropertiesPanel::~PropertiesPanel()
{
}

void PropertiesPanel::setWidget(NodeWidget *widget)
{
	if(m_widget != widget)
	{
		if(m_widget != NULL)
		{
			m_layout->removeWidget(m_widget);
			delete m_widget;
		}

		m_widget = widget;
		m_layout->addWidget(m_widget);
	}
}

void PropertiesPanel::selectionChanged(const QItemSelection& selected, const QItemSelection&)
{
	QModelIndexList sel = selected.indexes();

	Node* n = static_cast<Node*>(sel.first().internalPointer());
	setWidget(n->getWidget());
}

QSize PropertiesPanel::sizeHint() const
{
	return QSize(200, 600);
}
