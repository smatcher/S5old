#include <QtGui>

 #include "include/glwidget.h"
 #include "include/window.h"

 Window::Window(SceneGraph& sceneGraph)
 {
	 glWidget = new GLWidget(sceneGraph);

	 treeWidget = sceneGraph.getDebugView();

	 QHBoxLayout *mainLayout = new QHBoxLayout;
	 mainLayout->addWidget(glWidget);
	 mainLayout->addWidget(treeWidget);
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
