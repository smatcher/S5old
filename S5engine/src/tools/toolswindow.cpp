#include "tools/toolswindow.h"

#include <QtGui>

#include "tools/widgets/nodewidget.h"
#include "tools/widgets/propertiespanel.h"
#include "tools/widgets/renderwidget.h"
#include "tools/widgets/consolewidget.h"
#include "tools/mvc/scenegraphview.h"

#include "core/framework/engine.h"
#include "core/managers/cameramanager.h"
#include "core/utils/customevents.h"


ToolsWindow::ToolsWindow(Engine* engine)
{
	m_engine = engine;
	m_tab = new QTabWidget();
	m_renderWidget = RENDER_MANAGER.getDebugView();
	m_treeWidget = engine->getScenegraph_TEMPORARY()->getDebugView();
	m_propertiesWidget = new PropertiesPanel();

	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->addWidget(m_renderWidget);
	vlayout->addWidget(m_tab);
	setLayout(vlayout);

	QHBoxLayout* hlayout = new QHBoxLayout();
	QWidget* wid = new QFrame();
	hlayout->addWidget(m_treeWidget);
	hlayout->addWidget(m_propertiesWidget);
	wid->setLayout(hlayout);
	m_tab->addTab(wid, "Scene");

	hlayout = new QHBoxLayout();
	wid = new QFrame();
	wid->setLayout(hlayout);
	m_tab->addTab(wid, "Resources");

	wid = new ConsoleWidget();
	m_tab->addTab(wid, "Console");


	/*
	QGridLayout* layout = new QGridLayout();

	layout->addWidget(m_renderWidget,0,0,1,2);
	layout->addWidget(m_treeWidget,1,0);
	layout->addWidget(m_propertiesWidget,1,1);

	setLayout(layout);
	*/

	/*
	QDockWidget* dock1 = new QDockWidget("Editor");
	QDockWidget* dock2 = new QDockWidget("SceneTree");
	QDockWidget* dock3 = new QDockWidget("Properties");

	setCentralWidget(m_glWidget);
	dock1->setWidget(m_renderWidget);
	dock1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	dock2->setWidget(m_treeWidget);
	dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
	dock3->setWidget(m_propertiesWidget);
	dock3->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);

	addDockWidget(Qt::RightDockWidgetArea, dock1);
	addDockWidget(Qt::RightDockWidgetArea, dock2);
	addDockWidget(Qt::RightDockWidgetArea, dock3);
	*/

	QObject::connect(m_treeWidget->selectionModel(),
					 SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
					 m_propertiesWidget,
					 SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

	setWindowTitle(tr("S5 Debug"));
}

ToolsWindow::~ToolsWindow()
{
}

bool ToolsWindow::event(QEvent* evt)
{
	if(evt->type() == UPDATED_EVENT::type())
	{
		m_propertiesWidget->updateData();
		return true;
	}
	else
	{
		return QWidget::event(evt);
	}
}
