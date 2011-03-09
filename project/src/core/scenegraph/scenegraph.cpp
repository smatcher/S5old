#include "core/scenegraph/scenegraph.h"

SceneGraph::SceneGraph()
{
	m_debugView = NULL;
	m_debugModel = NULL;
}

SceneGraph::~SceneGraph()
{
	if(m_debugView != NULL)
		delete m_debugView;
}

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
}
