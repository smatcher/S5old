#ifndef WINDOW_H
#define WINDOW_H

#include <QMainWindow>
#include <QTreeWidget>

class GLWidget;
class RenderWidget;
class PropertiesPanel;
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
	void keyReleaseEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *evt);
	virtual bool event(QEvent* evt);

private:
	// widgets
	GLWidget*        m_glWidget;
    RenderWidget*    m_renderWidget;
    PropertiesPanel* m_propertiesWidget;
	QTreeView*       m_treeWidget;

	// engine
	Engine* m_engine;
};

#endif
