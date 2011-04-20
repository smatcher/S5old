#include "core/framework/engineloop.h"
#include "core/framework/engine.h"
#include "core/managers/updatemanager.h"
#include "core/managers/rendermanager.h"
#include "core/utils/customevents.h"

#include "core/resources/managers.h"
#include "core/properties/meshrenderer.h"

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
	RenderManager* renderManager = &(RENDER_MANAGER);
	UpdateManager* updateManager = &(UPDATE_MANAGER);

	m_gl->makeCurrent();
	renderManager->init(m_gl);
/*
	initResourceManagers();
*/
	QDateTime lastTime = QDateTime::currentDateTime();

	int i=0;
	while(m_engine->isRunning())
	{
		QDateTime time = QDateTime::currentDateTime();
		double elapsed = double(lastTime.msecsTo(time))/1000.0;

		updateManager->update(elapsed);
		renderManager->render(elapsed,m_engine->getScenegraph_TEMPORARY());

		lastTime = time;

		i++;

		if(i%10 == 0) {
			// Debug update
			QCoreApplication::postEvent(m_engine->m_scene.getDebugModel(),new UPDATED_EVENT());
			QCoreApplication::postEvent(&(m_engine->m_window),new UPDATED_EVENT());
		}

		QCoreApplication::processEvents();
	}
	m_gl->doneCurrent();
}
