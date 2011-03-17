#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class Camera;
class RenderWidget;
class CameraManager : public Manager<Camera>
{
public :
    CameraManager();
    ~CameraManager();

    virtual void onManageeAdded(Camera *managee);

    RenderWidget* getDebugView();

private :
    RenderWidget* m_widget;
};

typedef Singleton<CameraManager> CAMERA_MANAGER;

#endif // CAMERAMANAGER_H
