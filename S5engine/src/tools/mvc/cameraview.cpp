#include "tools/mvc/cameraview.h"
#include "tools/mvc/cameramodel.h"
#include "core/managers/cameramanager.h"

CameraView::CameraView()
{
	setModel(CAMERA_MANAGER.getDebugModel());

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
}

CameraView::~CameraView()
{
	CAMERA_MANAGER.clearDebug();
}

QSize CameraView::sizeHint() const
{
	return QSize(240,300);
}

void CameraView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	QModelIndexList list = deselected.indexes();

	list = selected.indexes();
	if(list.count() > 0)
	{
		Viewpoint* vp = CAMERA_MANAGER.getDebugModel()->getCamera(list.at(0));
		RENDER_MANAGER.setCurrentCamera(dynamic_cast<Camera*>(vp));
	}
	/*
	for(int i=0 ; i<list.size() ; i++) {
		Node* node = m_item->getDebugModel()->getNode(list.at(i));
		node->setSelected(true);
		m_item->getManipulator()->addSelected(node);
	}
	*/

	QListView::selectionChanged(selected, deselected);
}
