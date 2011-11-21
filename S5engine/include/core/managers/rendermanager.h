#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

#include "core/graphics/texture.h"
#include "core/maths/vector3.h"

class IRenderable;
class GLWidget;
class Viewpoint;
class Camera;
class SceneGraph;
class RenderTarget;

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
	Camera* m_camera;
	bool m_cameraChanged;

	bool m_drawDebug;
	DebugGizmosFilter m_drawDebugFilter;

	Background m_defaultBackground;

	QList<RenderTarget*> m_rts;

	void renderTarget(SceneGraph* sg, RenderTarget& target);
	void setupProjection(RenderTarget& target, int projection_nb);
	void applyBackground(RenderTarget& target, int projection_nb);

public:
	virtual ~RenderManager();

	void init(GLWidget* context);
	void render(double elapsed_time, SceneGraph* sg);

	void takeScreenshot(QString path="screen.png");

	void setCurrentCamera(Camera* cam);

	void setDrawDebug(bool draw);
	void setDrawDebugFilter(const DebugGizmosFilter& filter);
	bool getDrawDebug() const;
	DebugGizmosFilter getDrawDebugFilter() const;

	void setBackground(const Background& background);

	const Camera* getCurrentCamera();

	void addRenderTarget(RenderTarget* rt);
};

typedef Singleton<RenderManager> SingletonRenderManager;
#define RENDER_MANAGER SingletonRenderManager::getInstance()

#endif // RENDERMANAGER_H
