#ifndef TOOLSWINDOW_H
#define TOOLSWINDOW_H

#include <QWidget>
#include <QTreeWidget>
#include <QTableWidget>
#include <QIcon>
#include <QPushButton>

class RenderWidget;
class ResourcesWidget;
class PropertiesPanel;
class Engine;

class ToolsWindow : public QWidget
{
	Q_OBJECT

public:
	ToolsWindow(Engine* engine);
	virtual ~ToolsWindow();

	void enginePauseChanged(bool paused);

public slots:
	void pauseClicked();

protected:
	virtual bool event(QEvent* evt);

private:
	// widgets
	RenderWidget*    m_renderWidget;
	ResourcesWidget* m_resourcesWidget;
	QTabWidget*      m_tab;
	PropertiesPanel* m_propertiesWidget;
	QTreeView*       m_treeWidget;

	QIcon m_icon_pause;
	QIcon m_icon_play;
	QPushButton* m_pause_button;

	//engine
	Engine* m_engine;
};

#endif // TOOLSWINDOW_H
