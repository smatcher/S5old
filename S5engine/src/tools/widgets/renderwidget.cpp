#include "tools/widgets/renderwidget.h"
#include "core/managers/rendermanager.h"

#include "tools/mvc/cameramodel.h"
#include "tools/mvc/cameraview.h"
#include <QtGui>

RenderWidget::RenderWidget()
{
	/// DEBUG DRAW
	m_draw_debug_button = new QPushButton("Debug Gizmos");
	m_draw_debug_menu = new QMenu();

	m_draw_debug = m_draw_debug_menu->addAction("draw gizmos");
	m_draw_debug->setCheckable(true);
	m_draw_debug->setChecked(RENDER_MANAGER.getDrawDebug());
	m_draw_debug_menu->addSeparator();
	RenderManager::DebugGizmosFilter filter = RENDER_MANAGER.getDrawDebugFilter();
	m_draw_transforms = m_draw_debug_menu->addAction("transforms");
	m_draw_transforms->setCheckable(true);
	m_draw_transforms->setChecked(filter.draw_transforms);
	m_draw_lights = m_draw_debug_menu->addAction("lights");
	m_draw_lights->setCheckable(true);
	m_draw_lights->setChecked(filter.draw_lights);
	m_draw_cameras = m_draw_debug_menu->addAction("cameras");
	m_draw_cameras->setCheckable(true);
	m_draw_cameras->setChecked(filter.draw_cameras);
	m_draw_skeletons = m_draw_debug_menu->addAction("skeletons");
	m_draw_skeletons->setCheckable(true);
	m_draw_skeletons->setChecked(filter.draw_skeletons);
	m_draw_colliders = m_draw_debug_menu->addAction("colliders");
	m_draw_colliders->setCheckable(true);
	m_draw_colliders->setChecked(filter.draw_colliders);

	/// OPTIONS
	m_options_button = new QPushButton("Options");
	m_options_menu = new QMenu();

	m_options_shadows = m_options_menu->addAction("shadows");
	m_options_shadows->setCheckable(true);
	m_options_shadows->setChecked(RENDER_MANAGER.getShadowsEnabled());
	m_options_forward_pipeline = m_options_menu->addAction("Forward pipeline");
	m_options_forward_pipeline->setCheckable(true);
	m_options_deferred_pipeline = m_options_menu->addAction("Deferred pipeline");
	m_options_deferred_pipeline->setCheckable(true);
	QActionGroup* pipelinesGroup = new QActionGroup(m_options_menu);
	pipelinesGroup->addAction(m_options_forward_pipeline);
	pipelinesGroup->addAction(m_options_deferred_pipeline);
	if(RENDER_MANAGER.getRenderPipeline() == RenderManager::FORWARD_PIPELINE)
		m_options_forward_pipeline->setChecked(true);
	else
		m_options_deferred_pipeline->setChecked(true);

	/// CAMERAS
	m_cameras_combo = new QComboBox();
	m_cameras_combo->setModel(CAMERA_MANAGER.getDebugModel());
	m_cameras_combo->setView(CAMERA_MANAGER.getDebugView());

	QHBoxLayout* layout = new QHBoxLayout;
	layout->addWidget(m_draw_debug_button);
	layout->addWidget(m_options_button);
	layout->addWidget(m_cameras_combo);
	//layout->addStretch(1);
	setLayout(layout);

	setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);
	connect(m_draw_debug_button, SIGNAL(clicked()), this, SLOT(showDebugMenu()));
	connect(m_draw_debug, SIGNAL(triggered()), this, SLOT(drawDebugChanged()));
	connect(m_draw_colliders, SIGNAL(triggered()), this, SLOT(drawDebugFilterChanged()));
	connect(m_draw_cameras, SIGNAL(triggered()), this, SLOT(drawDebugFilterChanged()));
	connect(m_draw_transforms, SIGNAL(triggered()), this, SLOT(drawDebugFilterChanged()));
	connect(m_draw_lights, SIGNAL(triggered()), this, SLOT(drawDebugFilterChanged()));
	connect(m_draw_skeletons, SIGNAL(triggered()), this, SLOT(drawDebugFilterChanged()));

	connect(m_options_button, SIGNAL(clicked()), this, SLOT(showOptionsMenu()));
	connect(m_options_shadows, SIGNAL(triggered()), this, SLOT(optionShadowsChanged()));
	connect(m_options_forward_pipeline, SIGNAL(triggered()), this, SLOT(optionPipelineToForward()));
	connect(m_options_deferred_pipeline, SIGNAL(triggered()), this, SLOT(optionPipelineToDeferred()));
}

RenderWidget::~RenderWidget()
{
	RENDER_MANAGER.widgetDestroyed();
}

void RenderWidget::activeCameraChanged(Camera* cam)
{
	int index = 0;

	for(int i=0 ; i<CAMERA_MANAGER.managees().count() ; i++)
	{
		if(CAMERA_MANAGER.managees().at(i) == cam)
		{
			index = i+1;
			break;
		}
	}

	m_cameras_combo->setCurrentIndex(index);
}

void RenderWidget::setDrawDebug(bool draw)
{
	m_draw_debug->setChecked(draw);
}

void RenderWidget::drawDebugChanged()
{
	RENDER_MANAGER.setDrawDebug(m_draw_debug->isChecked());
}

void RenderWidget::drawDebugFilterChanged()
{
	RenderManager::DebugGizmosFilter filter;
	filter.draw_cameras = m_draw_cameras->isChecked();
	filter.draw_transforms = m_draw_transforms->isChecked();
	filter.draw_lights = m_draw_lights->isChecked();
	filter.draw_colliders = m_draw_colliders->isChecked();
	filter.draw_skeletons = m_draw_skeletons->isChecked();

	RENDER_MANAGER.setDrawDebugFilter(filter);
}

void RenderWidget::showDebugMenu()
{
	QPoint pos = QWidget::mapToGlobal(m_draw_debug_button->pos());
	pos.setY(pos.y() + m_draw_debug_button->height());
	m_draw_debug_menu->popup(pos);
}

void RenderWidget::setDrawDebugFilter(const RenderManager::DebugGizmosFilter& filter)
{
	m_draw_transforms->setChecked(filter.draw_transforms);
	m_draw_lights->setChecked(filter.draw_lights);
	m_draw_cameras->setChecked(filter.draw_cameras);
	m_draw_skeletons->setChecked(filter.draw_skeletons);
	m_draw_colliders->setChecked(filter.draw_colliders);
}

void RenderWidget::optionShadowsChanged()
{
	RENDER_MANAGER.setShadowsEnabled(m_options_shadows->isChecked());
}

void RenderWidget::optionPipelineToForward()
{
	RENDER_MANAGER.setRenderPipeline(RenderManager::FORWARD_PIPELINE);
}

void RenderWidget::optionPipelineToDeferred()
{
	RENDER_MANAGER.setRenderPipeline(RenderManager::DEFERRED_PIPELINE);
}

void RenderWidget::showOptionsMenu()
{
	QPoint pos = QWidget::mapToGlobal(m_options_button->pos());
	pos.setY(pos.y() + m_options_button->height());
	m_options_menu->popup(pos);
}

void RenderWidget::setShadowsEnabled(bool enabled)
{
	m_options_shadows->setChecked(enabled);
}

void RenderWidget::setRenderPipeline(RenderManager::RenderPipeline pipeline)
{
	if(pipeline == RenderManager::FORWARD_PIPELINE)
		m_options_forward_pipeline->setChecked(true);
	else
		m_options_deferred_pipeline->setChecked(true);
}
