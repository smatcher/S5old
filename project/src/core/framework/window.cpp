#include <QtGui>

#include "core/framework/window.h"
#include "core/framework/glwidget.h"
#include "core/framework/engine.h"

#include <iostream>

AppWindow::AppWindow(Engine* engine)
{
	m_engine = engine;
	m_glWidget = new GLWidget(this);

	setCentralWidget(m_glWidget);

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
