#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

#include "core/graphics/texture.h"
#include "core/graphics/shader.h"
#include "core/graphics/ubershader.h"
#include "core/graphics/material.h"
#include "core/graphics/framebufferobject.h"
#include "core/maths/vector3.h"

#include <QMatrix4x4>
#include <QVector2D>

class IRenderable;
class GLWidget;
class Viewpoint;
class Camera;
class SceneGraph;
class RenderTarget;
class RenderTexture;

class RenderManager : public Manager<IRenderable>
{
	friend class Singleton<RenderManager>;

public :

	enum BackgroundType
	{
		NO_CLEAR,
		COLOR,
		SINGLE_TEXTURE,
		SKYBOX
	};

	struct Background
	{
		BackgroundType type;
		Texture textures[6];
		Vector3f color;
	};

	enum RenderPassType
	{
		DEF_GEOMETRY_PASS,
		CAST_SHADOW_PASS,
		RECEIVE_SHADOW_PASS,
		FINAL_PASS
	};

	struct RenderPassInfo
	{
		const QGLWidget* context;
		RenderPassType type;
		bool setup_texture_matrices;
		bool background_enabled;
		bool texturing_enabled;
		UberShader ubershader_used;
	};

protected :
	RenderManager();

private :
	GLWidget* m_context;
	Camera* m_camera;
	bool m_cameraChanged;
	bool m_drawDebug;
	Background m_defaultBackground;

	QHash<QString, ShaderProgramData::UniformBase*> m_engine_uniforms;
	// engine uniforms data
	QMatrix4x4* m_inverse_modelview;
	QMatrix4x4* m_modelview;
	QMatrix4x4* m_inverse_projection;
	QMatrix4x4* m_projection;
	QVector2D* m_screen_size;
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

	UberShader m_deferred_geometry;
	UberShader m_deferred_ambient;
	UberShader m_deferred_lighting;
	UberShader m_generate_shadowmap;
	UberShader m_vertical_blur;
	UberShader m_horizontal_blur;
	UberShader m_bloom;
	UberShader m_depth;

	RenderTexture* m_bloommap;
	RenderTexture* m_colormap;

	// shadowmap render textures
	RenderTexture* m_shadowmap;
	FrameBufferObject* m_postprocessfbo;

	QList<RenderTarget*> m_rts;

	void clearTexture(RenderTexture* texture);
	void renderTarget(SceneGraph* sg, RenderTarget& target);
	void postprocessPass(RenderTexture* target_texture, QList<Texture> input_textures);
	void postprocessPass(QList< QPair<RenderTexture*, FrameBufferObject::AttachmentPoint> > target_textures, QList<Texture> input_textures);
	void debugDisplayTexture(Texture texture, int x, int y, int width, int height);
	void setupProjection(RenderTarget& target, int projection_nb);
	void applyBackground(RenderTarget& target, int projection_nb);

public:
	virtual ~RenderManager();

	void init(GLWidget* context);
	void createResources();
	void render(double elapsed_time, SceneGraph* sg);

	void setCurrentCamera(Camera* cam);
	void setDrawDebug(bool draw);

	void setBackground(const Background& background);

	const Camera* getCurrentCamera();

	void addRenderTarget(RenderTarget* rt);

	const QHash<QString, ShaderProgramData::UniformBase*>& getEngineUniforms();

	RenderPassInfo* getRenderPassInfo();
};

typedef Singleton<RenderManager> SingletonRenderManager;
#define RENDER_MANAGER SingletonRenderManager::getInstance()

#endif // RENDERMANAGER_H
