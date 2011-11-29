#include "tools/toolswindow.h"

#include <QtGui>

#include "tools/widgets/nodewidget.h"
#include "tools/widgets/propertiespanel.h"
#include "tools/widgets/renderwidget.h"
#include "tools/widgets/resourceswidget.h"
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
	m_resourcesWidget = new ResourcesWidget();

	m_icon_pause = QIcon("media/icons/pause.png");
	m_icon_play = QIcon("media/icons/play.png");

	if(engine->isPaused())
		m_pause_button = new QPushButton(m_icon_play,"");
	else
		m_pause_button = new QPushButton(m_icon_pause,"");
	m_pause_button->setMaximumWidth(40);

	QFrame* ctrl_frame = new QFrame();
	QHBoxLayout* hlayout = new QHBoxLayout();
	hlayout->addWidget(m_pause_button);
	hlayout->addWidget(m_renderWidget);
	ctrl_frame->setLayout(hlayout);

	QVBoxLayout* vlayout = new QVBoxLayout();
	vlayout->addWidget(ctrl_frame);
	vlayout->addWidget(m_tab);
	setLayout(vlayout);

	hlayout = new QHBoxLayout();
	QWidget* wid = new QFrame();
	hlayout->addWidget(m_treeWidget);
	hlayout->addWidget(m_propertiesWidget);
	wid->setLayout(hlayout);
	m_tab->addTab(wid, "Scene");

	m_tab->addTab(m_resourcesWidget, "Resources");

	wid = new ConsoleWidget();
	m_tab->addTab(wid, "Console");

	connect(m_pause_button, SIGNAL(clicked()), this, SLOT(pauseClicked()));

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

void ToolsWindow::pauseClicked()
{
	m_engine->setPaused(!m_engine->isPaused());
}

void ToolsWindow::enginePauseChanged(bool paused)
{
	if(paused)
		m_pause_button->setIcon(m_icon_play);
	else
		m_pause_button->setIcon(m_icon_pause);
}

