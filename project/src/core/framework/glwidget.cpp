#include "core/framework/glwidget.h"
#include "core/managers/updatemanager.h"

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

/*
 TODO : move to render thread

 void GLWidget::initializeGL()
 {
	 qglClearColor(Qt::black);

	 logo = new QtLogo(this, 64);
	 logo->setColor(qtGreen.dark());

	 glEnable(GL_DEPTH_TEST);
	 glEnable(GL_CULL_FACE);
	 glShadeModel(GL_SMOOTH);
	 glEnable(GL_LIGHTING);
	 glEnable(GL_LIGHT0);
	 glEnable(GL_MULTISAMPLE);
	 static GLfloat lightPosition[4] = { 1.5, 5.0, 7.0, 1.0 };
	 glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
 }

 void GLWidget::paintGL()
 {

	 glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	 glLoadIdentity();
	 glTranslatef(0.0, 0.0, -10.0);
	 glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
	 glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
	 glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

	 logo->draw();
	 glDisable(GL_LIGHTING);
	 for(int i=0 ; i<m_sg.childCount() ; i++) {
		 m_sg.child(i)->drawTransform(this,true);
	 }
	 glEnable(GL_LIGHTING);
 }

 void GLWidget::resizeGL(int width, int height)
 {
	 int side = qMin(width, height);
	 glViewport((width - side) / 2, (height - side) / 2, side, side);

	 glMatrixMode(GL_PROJECTION);
	 glLoadIdentity();
 #ifdef QT_OPENGL_ES_1
	 glOrthof(-5, +5, -5, +5, 1.0, 50.0);
 #else
	 glOrtho(-5, +5, -5, +5, 1.0, 50.0);
 #endif
	 glMatrixMode(GL_MODELVIEW);
 }
*/

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
	   e->type() == QEvent::Close)
	{
		return QGLWidget::event(e);
	}

	//std::cout << "GLWidget got event " << e->type() << std::endl;
	// L'event n'existe pas
	return true;

}

 void GLWidget::mousePressEvent(QMouseEvent *event)
 {
 }

 void GLWidget::mouseMoveEvent(QMouseEvent *event)
 {
	 m_camera.update(event->buttons() & Qt::LeftButton,
					 event->buttons() & Qt::RightButton,
					 event->buttons() & Qt::MiddleButton,
					 event->x(),
					 event->y());
 }

 void GLWidget::resizeEvent(QResizeEvent *evt)
 {
	 m_newSize = evt->size();
	 m_needResize = true;
	 //glt.resizeViewport(evt->size());
 }

 void GLWidget::paintEvent(QPaintEvent *)
 {
	 // Handled by an other thread.
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
