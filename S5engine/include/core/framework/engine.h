#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QApplication>
#include "core/scenegraph/scenegraph.h"
#include "core/framework/glwidget.h"
#include "core/framework/window.h"

#ifdef WITH_TOOLS
	#include "tools/toolswindow.h"
#endif

class Engine
{
private :
	QApplication m_app;

	SceneGraph m_scene;
	AppWindow  m_window;

	#ifdef WITH_TOOLS
		ToolsWindow m_toolswindow;
	#endif

	bool m_running;

	void init(int argc, char *argv[], QString mod_dir);

public :
	Engine(int argc, char *argv[], QString mod_dir = "");
	~Engine();

	int start();
	void stop();
	bool isRunning() {return m_running;}

	SceneGraph* getScenegraph_TEMPORARY() {return &m_scene;}
	GLWidget*   getGLW_TEMPORARY() {return m_window.getGLW_TEMPORARY();}
};

#endif // ENGINE_HPP
