#include "core/framework/engine.h"

#include "core/scenegraph/scenegraph.h"
#include "core/managers/updatemanager.h"
#include "core/maths/trigo.h"

#include <QDesktopWidget>

#include "core/resources/managers.h"
#include "core/resources/assimpfactory.h"
#include "core/resources/stbimage.h"

Engine::Engine(int argc, char *argv[]) :
	m_app(argc, argv),
	m_scene(),
	m_window(this),
	m_loopThread(this,m_window.getGLW_TEMPORARY()),
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

	MeshManager::getInstance().addFactory(new AssimpFactory()); // TODO : this is a memory leak, i don't like leaks, get rid of this leak.
	TextureManager::getInstance().addFactory(new StbImageFactory());

	MeshManager::getInstance().parseDir("../media/models",true);
	TextureManager::getInstance().parseDir("../media/textures",true);
	TextureManager::getInstance().loadAll();
}

int Engine::start()
{
	int ret;
	m_running = true;
//	m_loopThread.start();
	m_loopThread.run();

	ret = 0;
//	ret = m_app.exec();

	m_running = false;
	m_loopThread.wait();
	return ret;
}

void Engine::stop()
{
	m_running = false;
	m_loopThread.wait();
}
