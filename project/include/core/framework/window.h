#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTreeWidget>

class GLWidget;
class PropertiesPanel;
class Engine;

class AppWindow : public QWidget
{
	Q_OBJECT

public:
	AppWindow(Engine* engine);
	GLWidget* getGLW_TEMPORARY() {return m_glWidget;}

protected:
	void keyPressEvent(QKeyEvent *event);
	void closeEvent(QCloseEvent *evt);

private:
	// widgets
	GLWidget*        m_glWidget;
	PropertiesPanel* m_propertiesWidget;
	QTreeView*       m_treeWidget;

	// engine
	Engine* m_engine;
};

#endif
