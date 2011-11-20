#ifndef CAMERAVIEW_H
#define CAMERAVIEW_H

#include "QListView"

class CameraView : public QListView
{
	Q_OBJECT

public :
	CameraView();
	virtual ~CameraView();

	QSize sizeHint() const;

public slots:
	virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
};
#endif // CAMERAVIEW_H
