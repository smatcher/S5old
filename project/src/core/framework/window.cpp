#include <QtGui>

#include "debug/widgets/nodewidget.h"
#include "debug/widgets/propertiespanel.h"
#include "core/framework/window.h"
#include "core/framework/glwidget.h"

 Window::Window(SceneGraph& sceneGraph)
 {
	glWidget = new GLWidget(sceneGraph);
	treeWidget = sceneGraph.getDebugView();
	propertiesWidget = new PropertiesPanel();

	QHBoxLayout *mainLayout = new QHBoxLayout;
	mainLayout->addWidget(glWidget);
	mainLayout->addWidget(treeWidget);
	mainLayout->addWidget(propertiesWidget);
	setLayout(mainLayout);

	glWidget->setXRotation(15 * 16);
	glWidget->setYRotation(345 * 16);
	glWidget->setZRotation(0);

	QObject::connect(treeWidget->selectionModel(),
					 SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)),
					 propertiesWidget,
					 SLOT(selectionChanged(const QItemSelection&,const QItemSelection&)));

	setWindowTitle(tr("S5 Engine"));
 }

 void Window::keyPressEvent(QKeyEvent *e)
 {
	 if (e->key() == Qt::Key_Escape)
		 close();
	 else
		 QWidget::keyPressEvent(e);
 }
