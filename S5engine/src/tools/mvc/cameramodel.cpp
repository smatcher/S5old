#include "tools/mvc/cameramodel.h"
#include "core/utils/customevents.h"
#include "core/scenegraph/node.h"

CameraModel::CameraModel()
{
}

CameraModel::~CameraModel()
{
	CAMERA_MANAGER.clearDebug();
}

Viewpoint* CameraModel::getCamera(const QModelIndex &index) const
{
	/*
	if (index.isValid() && index.internalPointer() != this)
	{
		Viewpoint *item = static_cast<Viewpoint*>(index.internalPointer());
		if (item) return item;
	}
	*/
	if(index.isValid())
	{
		if(index.row() == 0)
			return CAMERA_MANAGER.getDebugCamera();
		else if(index.row() <= CAMERA_MANAGER.managees().count())
			return CAMERA_MANAGER.managees().at(index.row()-1);
	}

	return NULL;
}

/*
QModelIndex CameraModel::index(int row, int column, const QModelIndex& parent) const
{
	if (parent.isValid() && parent.column() != 0)
		return QModelIndex();

	Viewpoint *parentItem = getCamera(parent);

	if(parentItem != NULL)
	{
	return QModelIndex();
}
	else
	{
		Viewpoint* vp = NULL;

		if(row == 0)
		{
			vp = CAMERA_MANAGER.getDebugCamera();
		}
		else if(row <= CAMERA_MANAGER.managees().count() && row > 0)
		{
			vp = CAMERA_MANAGER.managees().at(row-1);
		}
		if (vp)
			return createIndex(row, column, vp);
		else
			return QModelIndex();
	}
}
*/

int CameraModel::rowCount(const QModelIndex& parent) const
{
	Viewpoint *parentItem = getCamera(parent);

	if(parentItem != NULL)
		return 0;
	else
		return 1+CAMERA_MANAGER.managees().count();
}

QVariant CameraModel::data(const QModelIndex& parent, int role) const
{
	if(role == Qt::DisplayRole)
	{
		Viewpoint* v = getCamera(parent);
		Camera* c = dynamic_cast<Camera*>(v);
		if(c != NULL){
			Node* n = c->node();
			if(n) return QVariant(c->node()->getName());
			else return QVariant(QString("unlinked cam"));
		}
		else if(v != NULL) return QVariant(QString("editor cam"));
	}
	return QVariant();
}

bool CameraModel::event(QEvent* evt)
{
	if(evt->type() == UPDATED_EVENT::type())
	{
		//emit dataChanged(QModelIndex(), QModelIndex());
		emit layoutChanged();
		return true;
	}
	else
	{
		return QAbstractItemModel::event(evt);
	}
}
