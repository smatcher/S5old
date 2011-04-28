#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>

class GLWidget;
class Engine;

class AppWindow : public QMainWindow
{
	Q_OBJECT

public:
	AppWindow(Engine* engine);
	virtual ~AppWindow();

	GLWidget* getGLW_TEMPORARY() {return m_glWidget;}

protected:
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *evt);

private:
	// widgets
	GLWidget*        m_glWidget;

	// engine
	Engine* m_engine;
};

#endif
