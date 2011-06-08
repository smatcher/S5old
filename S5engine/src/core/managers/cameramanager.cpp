#include "core/managers/cameramanager.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/renderwidget.h"
#endif

CameraManager::CameraManager()
{
	#ifdef WITH_TOOLS
		m_widget = NULL;
	#endif
}

CameraManager::~CameraManager()
{
}

#ifdef WITH_TOOLS

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

void CameraManager::widgetDestroyed()
{
	m_widget = NULL;
}

#endif
