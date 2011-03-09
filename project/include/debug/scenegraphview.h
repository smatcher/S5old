#ifndef SCENEGRAPHVIEW_H
#define SCENEGRAPHVIEW_H

#include "core/scenegraph/scenegraph.h"
#include "QTreeView"

class SceneGraph;

class SceneGraphView : public QTreeView
{
	Q_OBJECT

private :
	SceneGraph* m_item;
public :
	SceneGraphView(SceneGraph* item);
	virtual ~SceneGraphView();

	QSize sizeHint() const;
};

#endif // SCENEGRAPHVIEW_H
