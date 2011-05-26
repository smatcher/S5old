#include "tools/scenegraphview.h"
#include "tools/scenegraphmodel.h"

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
    return QSize(240,200);
}

