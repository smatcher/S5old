#include "core/scenegraph/scenegraph.h"

#ifdef WITH_TOOLS
	#include "tools/mvc/scenegraphview.h"
	#include "tools/mvc/scenegraphmodel.h"
#endif

SceneGraph::SceneGraph()
{
	#ifdef WITH_TOOLS
		m_debugView = 0;
		m_debugModel = 0;
	#endif
}

SceneGraph::~SceneGraph()
{
	#ifdef WITH_TOOLS
		if(m_debugView != 0)
			delete m_debugView;
	#endif
}

#ifdef WITH_TOOLS

SceneGraphView* SceneGraph::getDebugView()
{
	if(m_debugView == 0)
	{
		m_debugView = new SceneGraphView(this);
	}

	return m_debugView;
}

SceneGraphModel* SceneGraph::getDebugModel()
{
	if(m_debugModel == 0)
		m_debugModel = new SceneGraphModel(this);

	return m_debugModel;
}

void SceneGraph::clearDebug()
{
	m_debugView = 0;
	m_debugModel = 0;
}

void SceneGraph::setManipulator(Manipulator* manip)
{
	m_manipulator = manip;
}

Manipulator* SceneGraph::getManipulator()
{
	return m_manipulator;
}

#endif
