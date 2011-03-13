#include "core/framework/engineloop.h"
#include "core/framework/engine.h"
#include "core/managers/updatemanager.h"
#include "core/managers/rendermanager.h"

EngineLoop::EngineLoop(Engine* engine, GLWidget* gl) : QThread()
{
	m_gl = gl;
	m_engine = engine;
}

EngineLoop::~EngineLoop()
{
}

void EngineLoop::run()
{
	RenderManager* renderManager = ((RenderManager*)RenderManager::getInstance());
	UpdateManager* updateManager = ((UpdateManager*)UpdateManager::getInstance());

	renderManager->init(m_gl);

	while(m_engine->isRunning())
	{
		updateManager->update(1.0/60);
		renderManager->render(1000/60,m_engine->getScenegraph_TEMPORARY());
	}
}
