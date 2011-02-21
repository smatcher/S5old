#include "include/core/scenegraph/scenegraphview.h"

SceneGraphView::SceneGraphView(SceneGraph* item)
{
	m_item = item;
	setModel(m_item->getDebugModel());
	setHeaderHidden(true);
}

SceneGraphView::~SceneGraphView()
{
	m_item->clearDebug();
}
