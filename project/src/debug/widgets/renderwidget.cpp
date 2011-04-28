#include "debug/widgets/renderwidget.h"
#include "core/managers/rendermanager.h"
#include "debug/widgets/cameraradiobutton.h"
#include <QtGui>

RenderWidget::RenderWidget()
{
    QCheckBox* drawDebug = new QCheckBox("Draw debug info");
    QGroupBox* cameraBox = new QGroupBox("Rendered from camera");

    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(drawDebug);
    layout->addWidget(cameraBox);
	layout->addStretch(1);
	setLayout(layout);

    camerasLayout = new QVBoxLayout;
    cameraBox->setLayout(camerasLayout);
    CameraRadioButton* editorCam = new CameraRadioButton(NULL);
    editorCam->setChecked(true);
    camerasLayout->addWidget(editorCam);
	const QVector<Camera*>& cameras = CAMERA_MANAGER.managees();
    for(int i=0 ; i<cameras.count() ; i++)
    {
        CameraRadioButton* cam = new CameraRadioButton(cameras.at(i));
        camerasLayout->addWidget(cam);
    }
    //camerasLayout->addStretch(1);

    setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Minimum);
    connect(drawDebug, SIGNAL(stateChanged(int)), this, SLOT(drawDebugChanged(int)));
}

RenderWidget::~RenderWidget()
{
	CAMERA_MANAGER.widgetDestroyed();
}

void RenderWidget::cameraAdded(Camera *cam)
{
    CameraRadioButton* radio = new CameraRadioButton(cam);
    camerasLayout->addWidget(radio);
}

void RenderWidget::cameraRemoved(CameraRadioButton *radio)
{
	camerasLayout->removeWidget(radio);
	delete radio;
}

void RenderWidget::drawDebugChanged(int state)
{
    switch(state)
    {
        case Qt::Checked :
			RENDER_MANAGER.setDrawDebug(true);
            break;
        case Qt::Unchecked :
			RENDER_MANAGER.setDrawDebug(false);
            break;
        default :
            break;
    }
}

QSize RenderWidget::sizeHint() const
{
    return QSize(240,200);
}
