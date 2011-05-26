#include "tools/widgets/propertiespanel.h"
#include <QHBoxLayout>

PropertiesPanel::PropertiesPanel()
{
	m_widget = NULL;

	setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Minimum);
}

PropertiesPanel::~PropertiesPanel()
{
}

void PropertiesPanel::selectionChanged(const QItemSelection& selected, const QItemSelection&)
{
	QModelIndexList sel = selected.indexes();

	Node* n = static_cast<Node*>(sel.first().internalPointer());

    NodeWidget* widget = n->getWidget();

    if(m_widget != widget)
    {
        if(m_widget != NULL)
        {
            delete m_widget;
        }

        m_widget = widget;
        setWidget(m_widget);
    }
}

QSize PropertiesPanel::sizeHint() const
{
    return QSize(240, 200);
}

void PropertiesPanel::updateData()
{
	if(m_widget != NULL)
		m_widget->updateData();
}
