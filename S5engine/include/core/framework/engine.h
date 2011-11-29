#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QApplication>
#include "core/scenegraph/scenegraph.h"
#include "core/framework/glwidget.h"
#include "core/framework/window.h"

#ifdef WITH_TOOLS
	#include "tools/toolswindow.h"
	#include "tools/3D/manipulator.h"
#endif

class Engine
{
private :
	QApplication m_app;

	SceneGraph m_scene;
	AppWindow  m_window;

	bool m_running;
	bool m_paused;

	#ifdef WITH_TOOLS
		ToolsWindow m_toolswindow;
		Manipulator m_manipulator;
	#endif

	void init(int argc, char *argv[], QString mod_dir);

public :
	Engine(int argc, char *argv[], QString mod_dir = "");
	~Engine();

	int start();
	void stop();
	void setPaused(bool pause);

	bool isRunning() {return m_running;}
	bool isPaused() {return m_paused;}

	SceneGraph* getScenegraph_TEMPORARY() {return &m_scene;}
	GLWidget*   getGLW_TEMPORARY() {return m_window.getGLW_TEMPORARY();}

	static void MsgHandler(QtMsgType type, const char *msg);
};

#endif // ENGINE_HPP
