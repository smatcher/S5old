#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTreeWidget>

#include "include/core/scenegraph/scenegraph.h"

class QSlider;

class GLWidget;

class Window : public QWidget
{
	Q_OBJECT

public:
	Window(SceneGraph& sceneGraph);

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	GLWidget *glWidget;
	QTreeView* treeWidget;
};

#endif
