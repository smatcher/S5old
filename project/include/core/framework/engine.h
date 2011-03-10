#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QApplication>
#include <QThread>
#include "core/scenegraph/scenegraph.h"
#include "core/framework/glwidget.h"
#include "core/framework/window.h"

#include <stdio.h>

class EngineLoop : public QThread
{
	Q_OBJECT

private :
	GLWidget* m_gl;

signals :
	void needRedraw();

public :
	EngineLoop(GLWidget* gl) : QThread(), m_gl(gl)
	{
		connect(this, SIGNAL(needRedraw()),gl, SLOT(redraw()));
	}

	void run();
};

class Engine
{
private :
	QApplication m_app;

	SceneGraph m_scene;
	Window     m_window;

	EngineLoop m_loopThread;

	void init(int argc, char *argv[]);

public :
	Engine(int argc, char *argv[]);
	~Engine();

	int start();

	SceneGraph* getScenegraph_TEMPORARY() {return &m_scene;}
};

#endif // ENGINE_HPP
