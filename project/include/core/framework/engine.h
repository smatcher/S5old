#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <QApplication>
#include "core/scenegraph/scenegraph.h"
#include "core/framework/window.h"

class Engine : public QApplication
{
	Q_OBJECT

private :
	SceneGraph m_scene;
	Window     m_window;

	void init(int argc, char *argv[]);

public :
	Engine(int argc, char *argv[]);
	~Engine();

	int exec();

	SceneGraph* getScenegraph_TEMPORARY() {return &m_scene;}
};

#endif // ENGINE_HPP
