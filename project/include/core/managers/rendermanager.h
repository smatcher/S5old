#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class IRenderable;
class GLWidget;
class Camera;
class SceneGraph;

class RenderManager : public Manager<IRenderable>
{
	friend class Singleton<RenderManager>;

protected :
	RenderManager();

private :
	GLWidget* m_context;
	Camera* m_camera;
    bool m_cameraChanged;
    bool m_drawDebug;

	void setupProjection();

public:
	virtual ~RenderManager();

	void init(GLWidget* context);
	void render(double elapsed_time, SceneGraph* sg);

	void setCurrentCamera(Camera* cam);
	void setDrawDebug(bool draw);

	const Camera* getCurrentCamera();
};

typedef Singleton<RenderManager> SingletonRenderManager;
#define RENDER_MANAGER SingletonRenderManager::getInstance()

#endif // RENDERMANAGER_H
