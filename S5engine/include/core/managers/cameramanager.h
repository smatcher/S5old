#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class Camera;
class Viewpoint;

#ifdef WITH_TOOLS
	class CameraModel;
	class CameraView;
#endif

class CameraManager : public Manager<Camera>
{
public :
	CameraManager();
	~CameraManager();

	void setDebugCamera(Viewpoint* cam);
	Viewpoint* getDebugCamera();

#ifdef WITH_TOOLS

		CameraView* getDebugView();
		CameraModel* getDebugModel();
		void clearDebug();

	private :
		CameraView* m_debugView;
		CameraModel* m_debugModel;

		virtual void onManageeAdded(Camera *managee);
		virtual void onManageeRemoved(Camera *managee);
#endif

	private:
		Viewpoint* m_debug_camera;
};

typedef Singleton<CameraManager> SingletonCameraManager;
#define CAMERA_MANAGER SingletonCameraManager::getInstance()

#endif // CAMERAMANAGER_H
