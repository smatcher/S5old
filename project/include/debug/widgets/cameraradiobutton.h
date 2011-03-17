#ifndef CAMERARADIOBUTTON_H
#define CAMERARADIOBUTTON_H

#include "core/managers/rendermanager.h"
#include "core/properties/camera.h"
#include "core/scenegraph/node.h"
#include <QRadioButton>

class CameraRadioButton : public QRadioButton
{
    Q_OBJECT

private:
    Camera* m_camera;

public:
    CameraRadioButton(Camera* camera) : QRadioButton()
    {
        m_camera = camera;

        if(camera != NULL)
        {
            Node* node = camera->node();
            if(node != NULL)
                this->setText(node->getName());
            else
                this->setText("Unlinked camera");
        }
        else
        {
            this->setText("Editor cam");
        }

        connect(this, SIGNAL(toggled(bool)), this, SLOT(isToggled(bool)));
    }

public slots:
    void isToggled(bool checked)
    {
        if(checked)
            RENDER_MANAGER::getInstance().setCurrentCamera(m_camera);
    }
};

#endif // CAMERARADIOBUTTON_H
