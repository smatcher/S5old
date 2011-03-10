#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QTreeWidget>

#include "core/scenegraph/scenegraph.h"

class GLWidget;
class PropertiesPanel;

class Window : public QWidget
{
	Q_OBJECT

public:
	Window(SceneGraph& sceneGraph);
	GLWidget* getGLW_TEMPORARY() {return glWidget;}

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	GLWidget* glWidget;
	PropertiesPanel* propertiesWidget;
	QTreeView* treeWidget;
};

#endif
