#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

#include "core/graphics/texture.h"
#include "core/maths/vector3.h"

class IRenderable;
class GLWidget;
class Camera;
class SceneGraph;

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

protected :
	RenderManager();

private :
	GLWidget* m_context;
	Camera* m_camera;
    bool m_cameraChanged;
    bool m_drawDebug;
    Background m_defaultBackground;

    void setupProjection();
    void applyBackground();

public:
	virtual ~RenderManager();

	void init(GLWidget* context);
	void render(double elapsed_time, SceneGraph* sg);

	void setCurrentCamera(Camera* cam);
	void setDrawDebug(bool draw);

    void setBackground(const Background& background);

	const Camera* getCurrentCamera();
};

typedef Singleton<RenderManager> SingletonRenderManager;
#define RENDER_MANAGER SingletonRenderManager::getInstance()

#endif // RENDERMANAGER_H
