#ifndef ENGINELOOP_H
#define ENGINELOOP_H

#include <QThread>
#include "core/framework/glwidget.h"

class Engine;

class EngineLoop : public QThread
{
	Q_OBJECT

private :
	GLWidget* m_gl;
	Engine*   m_engine;

public :
	EngineLoop(Engine* engine, GLWidget* gl);
	virtual ~EngineLoop();
	void run();
};

#endif // ENGINELOOP_H
