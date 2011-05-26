#ifndef GLWIDGET_H
#define GLWIDGET_H

// #include <GL/glew.h> // pas besoin visiblement
#include <QGLWidget>
#include "core/scenegraph/scenegraph.h"

#ifdef WITH_TOOLS
	#include "tools/3D/debugcamera.h"
#endif

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	virtual ~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	bool needResize(QSize* size);
	void isResized();

	void applyCamera();
	void applyCameraRotation();

protected:
	bool event(QEvent * e);
	void resizeEvent(QResizeEvent *evt);
	void paintEvent(QPaintEvent *);
	void closeEvent(QCloseEvent *evt);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void keyPressEvent(QKeyEvent * event);
	void keyReleaseEvent(QKeyEvent * event);

private:
	QSize m_newSize;
	bool  m_needResize;

	#ifdef WITH_TOOLS
		DebugCamera m_camera;
	#endif
};

#endif
