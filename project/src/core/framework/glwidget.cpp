#include "core/framework/glwidget.h"
#include "core/managers/updatemanager.h"
#include "core/inputs/inputmanager.h"

#include <QtGui>
#include <QGLFormat>
#include <iostream>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

GLWidget::GLWidget(QWidget *parent)
		: QGLWidget(parent)
{
	m_needResize = false;

	setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Minimum);
	setFocusPolicy(Qt::StrongFocus);
	setAutoBufferSwap(false);

	QGLFormat _format = format();
	_format.setSwapInterval(1);
	setFormat(_format);
}

GLWidget::~GLWidget()
{
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(600, 600);
}

void GLWidget::applyCamera()
{
	m_camera.lookAt();
}

bool GLWidget::event(QEvent *e)
{
	if(e->type() == QEvent::MouseButtonPress ||
	   e->type() == QEvent::MouseButtonRelease ||
	   e->type() == QEvent::MouseMove ||
	   e->type() == QEvent::Resize ||
	   e->type() == QEvent::KeyPress ||
	   e->type() == QEvent::KeyRelease ||
	   e->type() == QEvent::Close)
	{
		return QGLWidget::event(e);
	}

	// L'event n'existe pas
	return true;
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	m_camera.update(event->buttons() & Qt::LeftButton,
					event->buttons() & Qt::RightButton,
					event->buttons() & Qt::MiddleButton,
					event->x(),
					event->y());
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	m_camera.update(event->buttons() & Qt::LeftButton,
					event->buttons() & Qt::RightButton,
					event->buttons() & Qt::MiddleButton,
					event->x(),
					event->y());
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	m_camera.update(event->buttons() & Qt::LeftButton,
					event->buttons() & Qt::RightButton,
					event->buttons() & Qt::MiddleButton,
					event->x(),
					event->y());
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		window()->close();
	else
	{
		INPUT_MANAGER::getInstance().reportButton(InputManager::Source_KeyBoard,InputManager::Held,e->key());
		QWidget::keyPressEvent(e);
	}
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
	{
		INPUT_MANAGER::getInstance().reportButton(InputManager::Source_KeyBoard,InputManager::Off,e->key());
		QWidget::keyReleaseEvent(e);
	}
}

void GLWidget::resizeEvent(QResizeEvent *evt)
{
	m_newSize = evt->size();
	m_needResize = true;
}

void GLWidget::paintEvent(QPaintEvent *)
{
	// Handled by the mainloop.
}

void GLWidget::closeEvent(QCloseEvent *evt)
{
	QGLWidget::closeEvent(evt);
}

bool GLWidget::needResize(QSize* size)
{
	*size = m_newSize;
	return m_needResize;
}

void GLWidget::isResized()
{
	m_needResize = false;
}
