#include <GL/glew.h>

#include "core/framework/engine.h"

#include "core/scenegraph/scenegraph.h"
#include "core/managers/rendermanager.h"
#include "core/managers/updatemanager.h"
#include "core/maths/trigo.h"

#include "core/utils/customevents.h"

#include <QDesktopWidget>
#include <QDateTime>

#include "core/resources/managers.h"

#ifdef WITH_TOOLS
	#include "tools/mvc/scenegraphmodel.h"
	#include "tools/3D/manipulator.h"
#endif

#include <AL/alut.h>

#include "core/utils/enginecommands.h"

Engine::Engine(int argc, char *argv[], QString mod_dir) :
	m_app(argc, argv),
	m_scene(),
	m_window(this),

	#ifdef WITH_TOOLS
		m_toolswindow(this),
	#endif

	m_running(false),
	m_paused(false)
{
	#ifndef WITH_TOOLS
		qInstallMsgHandler(Engine::MsgHandler);
	#endif
	init(argc, argv, mod_dir);
}

Engine::~Engine()
{
	m_scene.unlinkAll();
}

void Engine::init(int argc, char *argv[], QString mod_dir)
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

	glewInit();

	#ifdef WITH_TOOLS
		m_toolswindow.show();
		m_scene.setManipulator(&m_manipulator);
	#endif

	RENDER_MANAGER.setCurrentCamera(NULL);

	QCoreApplication::processEvents();

	Log::topicPolicy.insert("RESOURCE PARSING",Log::POLICY_IGNORE);
	initResourceManagers(mod_dir);

	registerEngineCommands(this);
}

int Engine::start()
{
	int ret = 0;
	m_running = true;

	RenderManager* renderManager = &(RENDER_MANAGER);
	UpdateManager* updateManager = &(UPDATE_MANAGER);

	renderManager->init(m_window.getGLW_TEMPORARY());

	QDateTime lastTime = QDateTime::currentDateTime();
	QDateTime lastFpsTime = QDateTime::currentDateTime();
	int nb_frames = 0;

	int i=0;
	while(m_running)
	{
		QDateTime time = QDateTime::currentDateTime();
		double elapsed = double(lastTime.msecsTo(time))/1000.0;

		if(!m_paused)
		{
			updateManager->update(elapsed);
		}

		renderManager->render(elapsed,&m_scene);

		lastTime = time;

		i++;
		nb_frames++;

		if(lastFpsTime.msecsTo(time) > 1000)
		{
			m_window.setWindowTitle("S5 - Engine : " + QString().setNum(nb_frames) + " FPS");
			lastFpsTime = time;
			nb_frames = 0;
		}

		#ifdef WITH_TOOLS
			// Debug update
			QCoreApplication::postEvent(&(m_toolswindow),new UPDATED_EVENT());
		#endif

		QCoreApplication::processEvents();
	}

	m_running = false;
	return ret;
}

void Engine::stop()
{
	m_running = false;
}

void Engine::setPaused(bool pause)
{
	m_paused = pause;
}

void Engine::MsgHandler(QtMsgType type, const char *msg)
{
	switch (type) {
		case QtDebugMsg:
			fprintf(stdout, "%s\n", msg);
			fflush(stdout);
			break;
		case QtWarningMsg:
			fprintf(stderr, "%s\n", msg);
			fflush(stderr);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "%s\n", msg);
			fflush(stderr);
			break;
		case QtFatalMsg:
			fprintf(stderr, "%s\n", msg);
			abort();
	}
}
