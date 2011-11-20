#ifndef CAMERAMODEL_H
#define CAMERAMODEL_H

#include "core/properties/camera.h"
#include <QAbstractListModel>
#include <QItemSelectionModel>

class Viewpoint;

class CameraModel : public QAbstractListModel
{
	Q_OBJECT

public:
	CameraModel();
	virtual ~CameraModel();

	//QModelIndex index(int row, int column, const QModelIndex &parent) const;
	int rowCount(const QModelIndex&) const;
	QVariant data(const QModelIndex&, int role) const;
	virtual bool event(QEvent* evt);

	Viewpoint* getCamera(const QModelIndex &index) const;
};

#endif
