#ifndef CAMERARADIOBUTTON_H
#define CAMERARADIOBUTTON_H

#include "core/managers/rendermanager.h"
#include "core/properties/camera.h"
#include "core/scenegraph/node.h"
#include "core/utils/customevents.h"
#include "debug/widgets/renderwidget.h"
#include <QRadioButton>

class CameraRadioButton : public QRadioButton
{
    Q_OBJECT

private:
    Camera* m_camera;

	void nameAfterCamera()
	{
		if(m_camera != NULL)
		{
			Node* node = m_camera->node();
			if(node != NULL)
				this->setText(node->getName());
			else
				this->setText("Unlinked camera");
		}
		else
		{
			this->setText("Editor cam");
		}
	}
public:
    CameraRadioButton(Camera* camera) : QRadioButton()
    {
		m_camera = camera;
		nameAfterCamera();

		if(m_camera != NULL)
			m_camera->setRadioButton(this);

        connect(this, SIGNAL(toggled(bool)), this, SLOT(isToggled(bool)));
	}

	virtual ~CameraRadioButton()
	{
		if(m_camera != NULL)
			m_camera->setRadioButton(this);
	}

public slots:
    void isToggled(bool checked)
    {
        if(checked)
			RENDER_MANAGER.setCurrentCamera(m_camera);
	}

	virtual bool event(QEvent* evt)
	{
		if(evt->type() == UPDATED_EVENT::type())
		{
			nameAfterCamera();
			return true;
		}
		else if(evt->type() == DELETED_EVENT::type())
		{
			((RenderWidget*)this->parentWidget())->cameraRemoved(this);
			return true;
		}
		else
		{
			return QRadioButton::event(evt);
		}
	}
};

#endif // CAMERARADIOBUTTON_H
