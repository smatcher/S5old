#ifndef DEBUGWINDOW_H
#define DEBUGWINDOW_H

#include <QWidget>
#include <QTreeWidget>

class RenderWidget;
class PropertiesPanel;
class Engine;

class DebugWindow : public QWidget
{
	Q_OBJECT

public:
	DebugWindow(Engine* engine);
	virtual ~DebugWindow();

protected:
	virtual bool event(QEvent* evt);

private:
	// widgets
	RenderWidget*    m_renderWidget;
	PropertiesPanel* m_propertiesWidget;
	QTreeView*       m_treeWidget;

	//engine
	Engine* m_engine;
};

#endif // DEBUGWINDOW_H
