#ifndef RENDERWIDGET_H
#define RENDERWIDGET_H

#include <core/managers/rendermanager.h>
#include <QWidget>
#include <QLayout>
#include <QCheckBox>
#include <QComboBox>

class Camera;

class RenderWidget : public QWidget
{
	Q_OBJECT

public :
	RenderWidget();
	virtual ~RenderWidget();

	void activeCameraChanged(Camera* cam);

	void setDrawDebug(bool draw);
	void setDrawDebugFilter(const RenderManager::DebugGizmosFilter& filter);

public slots :
	void drawDebugChanged();
	void drawDebugFilterChanged();
	void showDebugMenu();

private:
	QComboBox* m_cameras_combo;
	QPushButton* m_draw_debug_button;
	QMenu*   m_draw_debug_menu;
	QAction* m_draw_debug;
	QAction* m_draw_lights;
	QAction* m_draw_transforms;
	QAction* m_draw_skeletons;
	QAction* m_draw_cameras;
	QAction* m_draw_colliders;
};

#endif // RENDERWIDGET_H
