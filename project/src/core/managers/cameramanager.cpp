#include "core/managers/cameramanager.h"
#include "debug/widgets/renderwidget.h"

CameraManager::CameraManager() : m_widget(NULL)
{
}

CameraManager::~CameraManager()
{
}

void CameraManager::onManageeAdded(Camera *managee)
{
    if(m_widget != NULL)
        m_widget->cameraAdded(managee);
}

RenderWidget* CameraManager::getDebugView()
{
    if(m_widget == NULL)
        m_widget = new RenderWidget();

    return m_widget;
}
