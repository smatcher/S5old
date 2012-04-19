#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/abstraction/interface/irenderdevice.h"

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

#include "core/graphics/texture.h"
#include "core/graphics/framebufferobject.h"
#include "core/graphics/shader.h"
#include "core/graphics/ubershader.h"
#include "core/graphics/material.h"
#include "core/maths/vector3.h"
#include "core/maths/vector2.h"

#include <QMatrix4x4>
#include <QVector2D>

class IRenderable;
class GLWidget;
class Viewpoint;
class Camera;
class SceneGraph;
class RenderTarget;
class RenderTexture;

#ifdef WITH_TOOLS
	class RenderWidget;
#endif

class RenderManager : public Manager<IRenderable>
{
	friend class Singleton<RenderManager>;

	#ifdef WITH_TOOLS
		friend class RenderWidget;
	#endif

public :

	struct Background
	{
		enum Type
		{
			NO_CLEAR,
			COLOR,
			SINGLE_TEXTURE,
			SKYBOX
		};

		Background::Type type;
		Texture textures[6];
		Vector3f color;
		Vector3f sunDirection;
	};

	enum RenderPipeline
	{
		FORWARD_PIPELINE,
		DEFERRED_PIPELINE
	};

	struct RenderPassInfo
	{
		enum Type
		{
			DEF_GEOMETRY_PASS,
			DEF_LIGHTING_PASS,
			CAST_SHADOW_PASS,
			RECEIVE_SHADOW_PASS,
			FINAL_PASS
		};

		const QGLWidget* context;
		Type type;
		bool setup_texture_matrices;
		bool background_enabled;
		bool texturing_enabled;
		bool lighting_enabled;
		UberShader ubershader_used;
		Viewpoint* render_viewpoint;
		Viewpoint* lod_viewpoint;
	};

	struct DebugGizmosFilter
	{
		bool draw_transforms;
		bool draw_colliders;
		bool draw_lights;
		bool draw_cameras;
		bool draw_skeletons;

		DebugGizmosFilter() :
			draw_transforms(true),
			draw_colliders(true),
			draw_lights(true),
			draw_cameras(true),
			draw_skeletons(true) {}
	};

#ifdef WITH_TOOLS
		RenderWidget* getDebugView();

	private :
		RenderWidget* m_widget;
		void widgetDestroyed();
#endif

protected :
	RenderManager();

private :
	GLWidget* m_context;
	IRD::iRenderDevice* m_device;
	Camera* m_camera;
	bool m_cameraChanged;

	bool m_drawDebug;
	DebugGizmosFilter m_drawDebugFilter;

	Background m_defaultBackground;
	Vector2i m_viewport_size;

	QHash<QString, ShaderProgramData::UniformBase*> m_engine_uniforms;
	// engine uniforms data
	QMatrix4x4* m_inverse_modelview;
	QMatrix4x4* m_modelview;
	QMatrix4x4* m_inverse_projection;
	QMatrix4x4* m_projection;
	QMatrix4x4* m_texture_matrices;//8*6
	QVector2D* m_screen_size;
	QVector3D* m_sky_color;
	QVector2D* m_sun_pos;
	QVector3D* m_scene_ambient;

	// render pass info
	bool           m_rendering;
	RenderPassInfo m_passinfo;

	// deferred shading textures
	RenderTexture* m_positionmap;
	RenderTexture* m_normalmap;
	RenderTexture* m_diffusemap;
	RenderTexture* m_specularmap;
	RenderTexture* m_depthmap;
	RenderTexture* m_lightscatteringmap_high;
	RenderTexture* m_lightscatteringmap_low;

	UberShader m_deferred_geometry;
	UberShader m_deferred_ambient;
	UberShader m_deferred_lighting;
	UberShader m_forward;
	UberShader m_generate_shadowmap;
	UberShader m_vertical_blur;
	UberShader m_horizontal_blur;
	UberShader m_bloom;
	UberShader m_lightscattering;
	UberShader m_depth;

	RenderTexture* m_bloommap;
	RenderTexture* m_colormap;

	// shadowmap render textures
	RenderTexture* m_shadowmap;
	FrameBufferObject* m_postprocessfbo;
	FrameBufferObject* m_lowres_postprocessfbo;

	// SSS stuff
	RenderTexture* m_sssbuffer[3];
	FrameBufferObject* m_sss_fbo;

	QList<RenderTarget*> m_rts;

	QList<Texture> m_debugTextures;

	struct RendererOptions {
		RenderPipeline m_pipeline;
		bool           m_shadows_enabled;
		bool           m_bloom_enabled;
		bool           m_sss_enabled;
		bool           m_lightscattering_enabled;
		bool           m_normalmapping_enabled;
		bool           m_specularmapping_enabled;

		RendererOptions() :
			m_pipeline(DEFERRED_PIPELINE),
			m_shadows_enabled(true),
			m_bloom_enabled(false),
			m_sss_enabled(false),
			m_lightscattering_enabled(false),
			m_normalmapping_enabled(true),
			m_specularmapping_enabled(true)
		{}
	};
	RendererOptions m_options;

	void renderShadowmaps(SceneGraph* sg);
	void renderDeferred(SceneGraph* sg, Viewpoint* viewpoint);
	void renderForward(SceneGraph* sg, Viewpoint* viewpoint);

	void clearTexture(RenderTexture* texture);
	void renderTarget(SceneGraph* sg, RenderTarget& target);
	void drawDebug(SceneGraph* sg, RenderTarget& target);
	void postprocessPass(RenderTexture* target_texture, QList<Texture> input_textures, bool lowres = false);
	void postprocessPass(QList< QPair<RenderTexture*, IRD::FrameBuffer::Attachment> > target_textures, QList<Texture> input_textures, bool lowres = false);
	void postprocessPassOnFBO(RenderTexture* target_texture, QList<Texture> input_textures, FrameBufferObject* fbo);
	void postprocessPassOnFBO(QList< QPair<RenderTexture*, IRD::FrameBuffer::Attachment> > target_textures, QList<Texture> input_textures, FrameBufferObject* fbo);
	void debugDisplayTexture(Texture texture, int x, int y, int width, int height);
	void setupProjection(RenderTarget& target, int projection_nb);
	void applyBackground(RenderTarget& target, int projection_nb);

	void testViewportResize();
	void updateResources(int new_height, int new_width);

public:
	virtual ~RenderManager();

	void init(GLWidget* context);
	void createResources();
	void render(double elapsed_time, SceneGraph* sg);

	void takeScreenshot(QString path="screen.png");

	void setCurrentCamera(Camera* cam);
	void setAmbient(Vector3f ambient);

	void setShadowsEnabled(bool enabled);
	bool getShadowsEnabled();

	void setBloomEnabled(bool enabled);
	bool getBloomEnabled();

	void setSSSEnabled(bool enabled);
	bool getSSSEnabled();

	void setNormalMappingEnabled(bool enabled);
	bool getNormalMappingEnabled();

	void setSpecularMappingEnabled(bool enabled);
	bool getSpecularMappingEnabled();

	void setLightScatteringEnabled(bool enabled);
	bool getLightScatteringEnabled();

	void setRenderPipeline(RenderPipeline pipeline);
	RenderPipeline getRenderPipeline();

	void setDrawDebug(bool draw);
	void setDrawDebugFilter(const DebugGizmosFilter& filter);
	bool getDrawDebug() const;
	DebugGizmosFilter getDrawDebugFilter() const;

	void setBackground(const Background& background);
	Vector2i getCurrentViewportSize();

	void setTextureMatrix(QMatrix4x4 mat, int lightNb, int passNb);

	const Camera* getCurrentCamera();
	GLWidget*     getContext();

	void addRenderTarget(RenderTarget* rt);

	const QHash<QString, ShaderProgramData::UniformBase*>& getEngineUniforms();

	RenderPassInfo* getRenderPassInfo();

	IRD::iRenderDevice* getRenderDevice();

	void addDebugTexture(Texture texture);
	void clearDebugTextures();
};

typedef Singleton<RenderManager> SingletonRenderManager;
#define RENDER_MANAGER SingletonRenderManager::getInstance()

#endif // RENDERMANAGER_H
