#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class Camera;

#ifdef WITH_TOOLS
	class RenderWidget;
#endif

class CameraManager : public Manager<Camera>
{
	#ifdef WITH_TOOLS
		friend class RenderWidget;
	#endif

public :
	CameraManager();
	~CameraManager();

	#ifdef WITH_TOOLS

		RenderWidget* getDebugView();

	private :
		RenderWidget* m_widget;

		virtual void onManageeAdded(Camera *managee);
		void widgetDestroyed();

	#endif
};

typedef Singleton<CameraManager> SingletonCameraManager;
#define CAMERA_MANAGER SingletonCameraManager::getInstance()

#endif // CAMERAMANAGER_H
