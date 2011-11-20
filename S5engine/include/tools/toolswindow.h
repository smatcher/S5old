#ifndef TOOLSWINDOW_H
#define TOOLSWINDOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>

class RenderWidget;
class PropertiesPanel;
class Engine;

class ToolsWindow : public QWidget
{
	Q_OBJECT

public:
	ToolsWindow(Engine* engine);
	virtual ~ToolsWindow();

protected:
	virtual bool event(QEvent* evt);

private:
	// widgets
	RenderWidget*    m_renderWidget;
	QTabWidget*      m_tab;
	PropertiesPanel* m_propertiesWidget;
	QTreeView*       m_treeWidget;

	//engine
	Engine* m_engine;
};

#endif // TOOLSWINDOW_H
