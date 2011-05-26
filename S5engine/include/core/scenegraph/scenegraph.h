#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H

#include "core/utils/parenting.h"
#include "core/scenegraph/node.h"
#include "core/scenegraph/parentofnode.h"

#ifdef WITH_TOOLS
	class SceneGraphModel;
	class SceneGraphView;
#endif

class SceneGraph : public ParentOfNode
{		
public:
    SceneGraph();
	virtual ~SceneGraph();
	virtual ParentOfNode::TYPE type() {return ParentOfNode::SCENEGRAPH;}

#ifdef WITH_TOOLS
		SceneGraphView* getDebugView();
		SceneGraphModel* getDebugModel();
		void clearDebug();

	private :
		SceneGraphView* m_debugView;
		SceneGraphModel* m_debugModel;
#endif
};

#endif // SCENEGRAPH_H
