#include "debug/scenegraphview.h"

SceneGraphView::SceneGraphView(SceneGraph* item)
{
	m_item = item;
	setModel(m_item->getDebugModel());
	setHeaderHidden(true);

	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
}

SceneGraphView::~SceneGraphView()
{
	m_item->clearDebug();
}

QSize SceneGraphView::sizeHint() const
{
    return QSize(200,200);
}

