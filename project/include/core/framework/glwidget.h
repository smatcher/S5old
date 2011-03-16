#ifndef GLWIDGET_H
#define GLWIDGET_H

// #include <GL/glew.h> // pas besoin visiblement
#include <QGLWidget>
#include "core/scenegraph/scenegraph.h"

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(QWidget *parent = 0);
	virtual ~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	bool needResize(QSize* size);
	void isResized();

	int xRot;
	int yRot;
	int zRot;

protected:
	bool event(QEvent * e);
	void resizeEvent(QResizeEvent *evt);
	void paintEvent(QPaintEvent *);
	void closeEvent(QCloseEvent *evt);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	QPoint lastPos;

	QSize m_newSize;
	bool  m_needResize;
};

#endif
