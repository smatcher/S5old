#include "core/managers/cameramanager.h"
#include "core/utils/customevents.h"

#ifdef WITH_TOOLS
	#include "tools/mvc/cameraview.h"
	#include "tools/mvc/cameramodel.h"
#endif

CameraManager::CameraManager()
{
	m_debug_camera = NULL;

	#ifdef WITH_TOOLS
		m_debugView = NULL;
		m_debugModel = NULL;
	#endif
}

CameraManager::~CameraManager()
{
	#ifdef WITH_TOOLS
		if(m_debugView != NULL)
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
	if(m_debugView == NULL)
	{
		m_debugView = new CameraView();
	}

	return m_debugView;
}

CameraModel* CameraManager::getDebugModel()
{
	if(m_debugModel == NULL)
		m_debugModel = new CameraModel();

	return m_debugModel;
}

void CameraManager::clearDebug()
{
	m_debugView = NULL;
	m_debugModel = NULL;
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
