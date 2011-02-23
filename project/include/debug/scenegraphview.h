#ifndef SCENEGRAPHVIEW_H
#define SCENEGRAPHVIEW_H

#include "include/core/scenegraph/scenegraph.h"
#include "QTreeView"

class SceneGraph;

class SceneGraphView : public QTreeView
{
	Q_OBJECT

private :
	SceneGraph* m_item;
public :
	SceneGraphView(SceneGraph* item);
	~SceneGraphView();
};

#endif // SCENEGRAPHVIEW_H
