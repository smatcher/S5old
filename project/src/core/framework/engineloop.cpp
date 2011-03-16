#include "core/framework/engineloop.h"
#include "core/framework/engine.h"
#include "core/managers/updatemanager.h"
#include "core/managers/rendermanager.h"

#include <QDateTime>

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
	RenderManager* renderManager = &(RENDER_MANAGER::getInstance());
	UpdateManager* updateManager = &(UPDATE_MANAGER::getInstance());

	m_gl->makeCurrent();
	renderManager->init(m_gl);

	QDateTime lastTime = QDateTime::currentDateTime();

	while(m_engine->isRunning())
	{
		QDateTime time = QDateTime::currentDateTime();
		double elapsed = double(lastTime.msecsTo(time))/1000.0;

		updateManager->update(elapsed);
		renderManager->render(elapsed,m_engine->getScenegraph_TEMPORARY());

		lastTime = time;
	}
	m_gl->doneCurrent();
}
