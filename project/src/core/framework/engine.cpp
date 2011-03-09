#include "include/core/framework/engine.h"

#include "include/core/scenegraph/scenegraph.h"
#include "include/window.h"
#include "include/core/maths/trigo.h"

#include <QDesktopWidget>


Engine::Engine(int argc, char *argv[]) :
	QApplication(argc, argv),
	m_scene(),
	m_window(m_scene)
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

	m_window.resize(m_window.sizeHint());
	int desktopArea = QApplication::desktop()->width() *
					  QApplication::desktop()->height();
	int widgetArea = m_window.width() * m_window.height();

	if (((float)widgetArea / (float)desktopArea) < 0.75f)
		m_window.show();
	else
		m_window.showMaximized();
}

int Engine::exec()
{
	return QApplication::exec();
}
