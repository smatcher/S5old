#include "tools/widgets/renderwidget.h"
#include "core/managers/rendermanager.h"
#include "tools/widgets/cameraradiobutton.h"
#include <QtGui>

RenderWidget::RenderWidget()
{
	m_draw_debug_radio = new QCheckBox("Draw debug info");
	m_cameras_combo = new QComboBox();
	QGroupBox* cameraBox = new QGroupBox("Rendered from camera");

	QVBoxLayout* layout = new QVBoxLayout;
	layout->addWidget(m_draw_debug_radio);
	layout->addWidget(m_cameras_combo);
	layout->addWidget(cameraBox);
	layout->addStretch(1);
	setLayout(layout);

	camerasLayout = new QVBoxLayout;
	cameraBox->setLayout(camerasLayout);
	editorCam = new CameraRadioButton(NULL);
	camerasLayout->addWidget(editorCam);
	const QVector<Camera*>& cameras = CAMERA_MANAGER.managees();
	for(int i=0 ; i<cameras.count() ; i++)
	{
		CameraRadioButton* cam = new CameraRadioButton(cameras.at(i));
		camerasLayout->addWidget(cam);
	}
	//camerasLayout->addStretch(1);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	connect(m_draw_debug_radio, SIGNAL(stateChanged(int)), this, SLOT(drawDebugChanged(int)));
}

RenderWidget::~RenderWidget()
{
	CAMERA_MANAGER.widgetDestroyed();
}

void RenderWidget::cameraAdded(Camera *cam)
{
	CameraRadioButton* radio = new CameraRadioButton(cam);
	camerasLayout->addWidget(radio);

	Node* node = cam->node();

	if(node != NULL) {
		m_cameras_combo->insertItem(0,node->getName());
	}
}

void RenderWidget::cameraRemoved(CameraRadioButton *radio)
{
	camerasLayout->removeWidget(radio);
	delete radio;
}

void RenderWidget::activeCameraChanged(Camera* cam)
{
	if(cam == NULL)
	{
		if(! editorCam->isChecked())
			editorCam->setChecked(true);
	}
	else
	{
		CameraRadioButton* radio = cam->getRadioButton();

		if(radio != NULL)
		{
			if(! radio->isChecked())
				radio->setChecked(true);
		}
	}
}

void RenderWidget::setDrawDebug(bool draw)
{
	m_draw_debug_radio->setChecked(draw);
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
