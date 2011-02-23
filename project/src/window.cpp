#include <QtGui>

 #include "include/debug/widgets/nodewidget.h"
 #include "include/glwidget.h"
 #include "include/window.h"

 Window::Window(SceneGraph& sceneGraph)
 {
	 glWidget = new GLWidget(sceneGraph);
	 nodeWidget = sceneGraph.child(0)->getWidget();
	 treeWidget = sceneGraph.getDebugView();

	 QHBoxLayout *mainLayout = new QHBoxLayout;
	 mainLayout->addWidget(glWidget);
	 mainLayout->addWidget(treeWidget);
	 mainLayout->addWidget(nodeWidget);
	 setLayout(mainLayout);

	glWidget->setXRotation(15 * 16);
	glWidget->setYRotation(345 * 16);
	glWidget->setZRotation(0);

	 setWindowTitle(tr("Hello GL"));
 }

 void Window::keyPressEvent(QKeyEvent *e)
 {
	 if (e->key() == Qt::Key_Escape)
		 close();
	 else
		 QWidget::keyPressEvent(e);
 }
