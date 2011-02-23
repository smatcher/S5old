#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTreeWidget>

#include "include/core/scenegraph/scenegraph.h"

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
	NodeWidget* nodeWidget;
	QTreeView* treeWidget;
};

#endif
