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
	void setShadowsEnabled(bool enabled);
	void setBloomEnabled(bool enabled);
	void setSSSEnabled(bool enabled);
	void setNormalMappingEnabled(bool enabled);
	void setSpecularMappingEnabled(bool enabled);
	void setLightScatteringEnabled(bool enabled);
	void setRenderPipeline(RenderManager::RenderPipeline pipeline);

public slots :
	void drawDebugChanged();
	void drawDebugFilterChanged();
	void showDebugMenu();

	void optionShadowsChanged();
	void optionBloomChanged();
	void optionSSSChanged();
	void optionNormalMappingChanged();
	void optionSpecularMappingChanged();
	void optionLightScatteringChanged();
	void optionPipelineToForward();
	void optionPipelineToDeferred();
	void showOptionsMenu();

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

	QPushButton* m_options_button;
	QMenu*   m_options_menu;
	QAction* m_options_forward_pipeline;
	QAction* m_options_deferred_pipeline;
	QAction* m_options_shadows;
	QAction* m_options_bloom;
	QAction* m_options_sss;
	QAction* m_options_lightscattering;
	QAction* m_options_normalmapping;
	QAction* m_options_specularmapping;
};

#endif // RENDERWIDGET_H
