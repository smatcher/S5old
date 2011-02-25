#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "include/core/utils/parenting.h"
#include "include/core/scenegraph/node.h"

#include "include/debug/scenegraphview.h"
#include "include/debug/scenegraphmodel.h"

class SceneGraphModel;
class SceneGraphView;

class SceneGraph : public ParentOf<Node>
{		
public:
    SceneGraph();
	virtual ~SceneGraph();

	SceneGraphView* getDebugView();
	SceneGraphModel* getDebugModel();
	void clearDebug();

private :
	SceneGraphView* m_debugView;
	SceneGraphModel* m_debugModel;
};

#endif // SCENEGRAPH_H
