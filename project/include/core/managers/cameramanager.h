#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class Camera;
class RenderWidget;
class CameraManager : public Manager<Camera>
{
	friend class RenderWidget;

public :
    CameraManager();
    ~CameraManager();

    RenderWidget* getDebugView();

private :
    RenderWidget* m_widget;

	virtual void onManageeAdded(Camera *managee);
	void widgetDestroyed();
};

typedef Singleton<CameraManager> CAMERA_MANAGER;

#endif // CAMERAMANAGER_H
