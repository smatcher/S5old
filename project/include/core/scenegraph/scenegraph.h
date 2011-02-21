#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "include/core/utils/parenting.h"
#include "include/core/scenegraph/node.h"

#include "include/core/scenegraph/scenegraphview.h"
#include "include/core/scenegraph/scenegraphmodel.h"

class SceneGraphModel;
class SceneGraphView;

class SceneGraph : public ParentOf<Node>
{		
public:
    SceneGraph();
	~SceneGraph();

	SceneGraphView* getDebugView();
	SceneGraphModel* getDebugModel();
	void clearDebug();

private :
	SceneGraphView* m_debugView;
	SceneGraphModel* m_debugModel;
};

#endif // SCENEGRAPH_H
