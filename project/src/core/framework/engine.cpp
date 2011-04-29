#include "core/framework/engine.h"

#include "core/scenegraph/scenegraph.h"
#include "core/managers/rendermanager.h"
#include "core/managers/updatemanager.h"
#include "core/maths/trigo.h"

#include "core/utils/customevents.h"

#include <QDesktopWidget>
#include <QDateTime>

#include "core/resources/managers.h"

#include <AL/alut.h>

Engine::Engine(int argc, char *argv[]) :
	m_app(argc, argv),
	m_scene(),
	m_window(this),
	m_debugWindow(this),
	m_running(false)
{
	init(argc, argv);
}

Engine::~Engine()
{
	m_scene.unlinkAll();
}

void Engine::init(int argc, char *argv[])
{
	initTrigo();

	m_app.setStyle("Plastique");

	m_window.resize(m_window.sizeHint());
	int desktopArea = QApplication::desktop()->width() *
					  QApplication::desktop()->height();
	int widgetArea = m_window.width() * m_window.height();

	if (((float)widgetArea / (float)desktopArea) < 0.75f)
		m_window.show();
	else
		m_window.showMaximized();

	alutInit(&argc, argv);
	m_window.getGLW_TEMPORARY()->makeCurrent();

	m_debugWindow.show();

	initResourceManagers();
}

int Engine::start()
{
	int ret = 0;
	m_running = true;

	RenderManager* renderManager = &(RENDER_MANAGER);
	UpdateManager* updateManager = &(UPDATE_MANAGER);

	renderManager->init(m_window.getGLW_TEMPORARY());
/*
	initResourceManagers();
*/
	QDateTime lastTime = QDateTime::currentDateTime();

	int i=0;
	while(m_running)
	{
		QDateTime time = QDateTime::currentDateTime();
		double elapsed = double(lastTime.msecsTo(time))/1000.0;

		updateManager->update(elapsed);
		renderManager->render(elapsed,&m_scene);

		lastTime = time;

		i++;

		if(i%10 == 0) {
			// Debug update
			QCoreApplication::postEvent(m_scene.getDebugModel(),new UPDATED_EVENT());
			QCoreApplication::postEvent(&(m_window),new UPDATED_EVENT());
		}

		QCoreApplication::processEvents();
	}

	m_running = false;
	return ret;
}

void Engine::stop()
{
	m_running = false;
}
