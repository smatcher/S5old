#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QApplication>
#include "core/scenegraph/scenegraph.h"
#include "core/framework/glwidget.h"
#include "core/framework/window.h"
#include "core/framework/engineloop.h"

class Engine
{
private :
	QApplication m_app;

	SceneGraph m_scene;
	AppWindow  m_window;

	EngineLoop m_loopThread;

	bool m_running;

	void init(int argc, char *argv[]);

public :
	Engine(int argc, char *argv[]);
	~Engine();

	int start();
	void stop();
	bool isRunning() {return m_running;}

	SceneGraph* getScenegraph_TEMPORARY() {return &m_scene;}
	GLWidget*   getGLW_TEMPORARY() {return m_window.getGLW_TEMPORARY();}
};

#endif // ENGINE_HPP
