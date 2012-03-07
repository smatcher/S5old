#include "core/managers/cameramanager.h"
#include "core/utils/customevents.h"

#ifdef WITH_TOOLS
	#include "tools/mvc/cameraview.h"
	#include "tools/mvc/cameramodel.h"
#endif

CameraManager::CameraManager()
{
	m_debug_camera = 0;

	#ifdef WITH_TOOLS
		m_debugView = 0;
		m_debugModel = 0;
	#endif
}

CameraManager::~CameraManager()
{
	#ifdef WITH_TOOLS
		if(m_debugView != 0)
			delete m_debugView;
	#endif
}

void CameraManager::setDebugCamera(Viewpoint *cam)
{
	m_debug_camera = cam;
}

Viewpoint* CameraManager::getDebugCamera()
{
	return m_debug_camera;
}

#ifdef WITH_TOOLS

CameraView* CameraManager::getDebugView()
{
	if(m_debugView == 0)
	{
		m_debugView = new CameraView();
	}

	return m_debugView;
}

CameraModel* CameraManager::getDebugModel()
{
	if(m_debugModel == 0)
		m_debugModel = new CameraModel();

	return m_debugModel;
}

void CameraManager::clearDebug()
{
	m_debugView = 0;
	m_debugModel = 0;
}

void CameraManager::onManageeAdded(Camera *managee)
{
	QCoreApplication::postEvent(CAMERA_MANAGER.getDebugModel(), new UPDATED_EVENT());
}

void CameraManager::onManageeRemoved(Camera *managee)
{
	QCoreApplication::postEvent(CAMERA_MANAGER.getDebugModel(), new UPDATED_EVENT());
}

#endif
