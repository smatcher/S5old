#include <QtGui>

#include "debug/widgets/nodewidget.h"
#include "debug/widgets/propertiespanel.h"
#include "debug/widgets/renderwidget.h"
#include "core/framework/window.h"
#include "core/framework/glwidget.h"
#include "core/framework/engine.h"
#include "core/managers/cameramanager.h"
#include "core/utils/customevents.h"

#include <iostream>

AppWindow::AppWindow(Engine* engine)
{
	m_engine = engine;
	m_glWidget = new GLWidget(this);
	m_renderWidget = CAMERA_MANAGER::getInstance().getDebugView();
	m_treeWidget = engine->getScenegraph_TEMPORARY()->getDebugView();
	m_propertiesWidget = new PropertiesPanel();

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

	QObject::connect(m_treeWidget->selectionModel(),
					 SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
					 m_propertiesWidget,
					 SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

	setWindowTitle(tr("S5 Engine"));
 }

AppWindow::~AppWindow()
{
}

void AppWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
}

void AppWindow::closeEvent(QCloseEvent *evt)
{
	m_engine->stop();
	evt->accept();
}

bool AppWindow::event(QEvent* evt)
{
	if(evt->type() == UPDATED_EVENT::type())
	{
		m_propertiesWidget->updateData();
		return true;
	}
	else
	{
		return QMainWindow::event(evt);
	}
}
