#include "core/framework/engine.h"

#include "core/scenegraph/scenegraph.h"
#include "core/managers/updatemanager.h"
#include "core/maths/trigo.h"

#include <QDesktopWidget>

void EngineLoop::run()
{
	forever
	{
		msleep(1000/60);
		((UpdateManager*)UpdateManager::getInstance())->update(1.0/60);
		emit needRedraw();
		//m_gl->glupdate();
	}
}

Engine::Engine(int argc, char *argv[]) :
	m_app(argc, argv),
	m_scene(),
	m_window(m_scene),
	m_loopThread(m_window.getGLW_TEMPORARY())
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

}

int Engine::start()
{
	int ret;
	m_loopThread.start();
	ret = m_app.exec();
	m_loopThread.wait();
	return ret;
}
