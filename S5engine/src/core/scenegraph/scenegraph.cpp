#include "core/scenegraph/scenegraph.h"

#ifdef WITH_TOOLS
	#include "tools/scenegraphview.h"
	#include "tools/scenegraphmodel.h"
#endif

SceneGraph::SceneGraph()
{
	#ifdef WITH_TOOLS
		m_debugView = NULL;
		m_debugModel = NULL;
	#endif
}

SceneGraph::~SceneGraph()
{
	#ifdef WITH_TOOLS
		if(m_debugView != NULL)
			delete m_debugView;
	#endif
}

#ifdef WITH_TOOLS

SceneGraphView* SceneGraph::getDebugView()
{
	if(m_debugView == NULL)
	{
		m_debugView = new SceneGraphView(this);
	}

	return m_debugView;
}

SceneGraphModel* SceneGraph::getDebugModel()
{
	if(m_debugModel == NULL)
		m_debugModel = new SceneGraphModel(this);

	return m_debugModel;
}

void SceneGraph::clearDebug()
{
	m_debugView = NULL;
	m_debugModel = NULL;
}

#endif
