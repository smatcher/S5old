#include <QtGui>

#include "debug/widgets/nodewidget.h"
#include "debug/widgets/propertiespanel.h"
#include "core/framework/window.h"
#include "core/framework/glwidget.h"
#include "core/framework/engine.h"

#include <iostream>

 AppWindow::AppWindow(Engine* engine)
 {
	m_engine = engine;
	m_glWidget = new GLWidget(this);
	m_treeWidget = engine->getScenegraph_TEMPORARY()->getDebugView();
	m_propertiesWidget = new PropertiesPanel();

	QDockWidget* dock1 = new QDockWidget();
	QDockWidget* dock2 = new QDockWidget();

dock1->setWidget(m_treeWidget);
dock2->setWidget(m_propertiesWidget);

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(m_glWidget);
	mainLayout->addWidget(dock1);
	mainLayout->addWidget(dock2);
//	mainLayout->addWidget(m_treeWidget);
//	mainLayout->addWidget(m_propertiesWidget);
	setLayout(mainLayout);


	m_glWidget->setXRotation(15 * 16);
	m_glWidget->setYRotation(345 * 16);
	m_glWidget->setZRotation(0);

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
	else
		QWidget::keyPressEvent(e);
}

void AppWindow::closeEvent(QCloseEvent *evt)
{
	m_engine->stop();
	evt->accept();
//	QWidget::closeEvent(evt);
}
