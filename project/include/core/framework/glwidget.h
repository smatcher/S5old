#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <GL/glew.h>
#include <QGLWidget>
#include "core/scenegraph/scenegraph.h"

class QtLogo;
class SceneGraph;

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	GLWidget(const SceneGraph&, QWidget *parent = 0);
	virtual ~GLWidget();

	QSize minimumSizeHint() const;
	QSize sizeHint() const;

public slots:
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

signals:
	void xRotationChanged(int angle);
	void yRotationChanged(int angle);
	void zRotationChanged(int angle);

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);

private:
	QtLogo *logo;
	const SceneGraph& m_sg;
	int xRot;
	int yRot;
	int zRot;
	QPoint lastPos;
	QColor qtGreen;
	QColor qtPurple;
};

#endif
