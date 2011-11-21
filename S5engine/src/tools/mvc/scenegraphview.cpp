#include "tools/mvc/scenegraphview.h"
#include "tools/mvc/scenegraphmodel.h"
#include "tools/3D/manipulator.h"

#include <QModelIndexList>
#include <QList>

SceneGraphView::SceneGraphView(SceneGraph* item)
{
	m_item = item;
	setModel(m_item->getDebugModel());
	setHeaderHidden(true);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

SceneGraphView::~SceneGraphView()
{
	m_item->clearDebug();
}

QSize SceneGraphView::sizeHint() const
{
	return QSize(240,300);
}

void SceneGraphView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	m_item->getManipulator()->unselectAll();

	QModelIndexList list = deselected.indexes();
	for(int i=0 ; i<list.size() ; i++) {
		Node* node = m_item->getDebugModel()->getNode(list.at(i));
		node->setSelected(false);
	}

	list = selected.indexes();
	for(int i=0 ; i<list.size() ; i++) {
		Node* node = m_item->getDebugModel()->getNode(list.at(i));
		node->setSelected(true);
		m_item->getManipulator()->addSelected(node);
	}

	QTreeView::selectionChanged(selected, deselected);
}
