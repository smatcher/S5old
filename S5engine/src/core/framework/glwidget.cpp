#include "core/framework/glwidget.h"
#include "core/managers/updatemanager.h"
#include "core/managers/rendermanager.h"
#include "core/inputs/inputmanager.h"

#include <QtGui>
#include <QGLFormat>
#include <iostream>

#include <QtConcurrentRun>

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

void saveWrapper(const QPixmap& pix, const QString& path)
{
	if(pix.save(path))
	{
		logInfo("Screenshot saved to " + path);
	}
	else
	{
		logError("Screenshot not saved, wrong format");
	}
}

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

void GLWidget::takeScreenshot(QString path)
{
	QImage screen = grabFrameBuffer();
	QPixmap shot = QPixmap::fromImage(screen);
	QtConcurrent::run(&saveWrapper,shot,path);
}

QSize GLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize GLWidget::sizeHint() const
{
	return QSize(640, 480);
}

Viewpoint* GLWidget::getViewpoint()
{
	#ifdef WITH_TOOLS
		return &m_camera;
	#else
		return 0;
	#endif
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
	if(RENDER_MANAGER.getCurrentCamera() == 0)
	{
		#ifdef WITH_TOOLS
			m_camera.updateMouse(event->buttons() & Qt::LeftButton,
					event->buttons() & Qt::RightButton,
					event->buttons() & Qt::MiddleButton,
					event->x(),
					event->y());
		#endif
	}
	else
	{
	}
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	if(RENDER_MANAGER.getCurrentCamera() == 0)
	{
		#ifdef WITH_TOOLS
			m_camera.updateMouse(event->buttons() & Qt::LeftButton,
					event->buttons() & Qt::RightButton,
					event->buttons() & Qt::MiddleButton,
					event->x(),
					event->y());
		#endif
	}
	else
	{
	}
}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	if(RENDER_MANAGER.getCurrentCamera() == 0)
	{
		#ifdef WITH_TOOLS
			m_camera.updateMouse(event->buttons() & Qt::LeftButton,
					event->buttons() & Qt::RightButton,
					event->buttons() & Qt::MiddleButton,
					event->x(),
					event->y());
		#endif
	}
	else
	{
	}
}

void GLWidget::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		window()->close();
	else
	{
		if(RENDER_MANAGER.getCurrentCamera() == 0)
		{
			#ifdef WITH_TOOLS
				int key = e->key();
				switch(key)
				{
					case Qt::Key_Up :
						m_camera.updateKeyboard(DebugCamera::UP,true);
						break;
					case Qt::Key_Down :
						m_camera.updateKeyboard(DebugCamera::DOWN,true);
						break;
					case Qt::Key_Left :
						m_camera.updateKeyboard(DebugCamera::LEFT,true);
						break;
					case Qt::Key_Right :
						m_camera.updateKeyboard(DebugCamera::RIGHT,true);
						break;
					case Qt::Key_Shift :
						m_camera.updateKeyboard(DebugCamera::RSHIFT,true);
						break;
					case Qt::Key_Control :
						m_camera.updateKeyboard(DebugCamera::RCTRL,true);
						break;
				}
			#endif

			if(e->key() == Qt::Key_F12) {
				takeScreenshot();
			}
		}
		else
		{
			INPUT_MANAGER.reportButton(InputManager::Source_KeyBoard,InputManager::Held,e->key());
			QWidget::keyPressEvent(e);
		}
	}
}

void GLWidget::keyReleaseEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
	{
		if(RENDER_MANAGER.getCurrentCamera() == 0)
		{
			#ifdef WITH_TOOLS
				int key = e->key();
				switch(key)
				{
					case Qt::Key_Up :
						m_camera.updateKeyboard(DebugCamera::UP,false);
						break;
					case Qt::Key_Down :
						m_camera.updateKeyboard(DebugCamera::DOWN,false);
						break;
					case Qt::Key_Left :
						m_camera.updateKeyboard(DebugCamera::LEFT,false);
						break;
					case Qt::Key_Right :
						m_camera.updateKeyboard(DebugCamera::RIGHT,false);
						break;
					case Qt::Key_Shift :
						m_camera.updateKeyboard(DebugCamera::RSHIFT,false);
						break;
					case Qt::Key_Control :
						m_camera.updateKeyboard(DebugCamera::RCTRL,false);
						break;
				}
			#endif

		}
		else
		{
			INPUT_MANAGER.reportButton(InputManager::Source_KeyBoard,InputManager::Off,e->key());
			QWidget::keyReleaseEvent(e);
		}
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
