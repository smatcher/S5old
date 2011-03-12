#ifndef RENDERMANAGER_H
#define RENDERMANAGER_H

#include "core/managers/manager.h"

class IRenderable;
class GLWidget;
class SceneGraph;

class RenderManager : public Manager<IRenderable>
{
private :
	GLWidget* m_context;

public:
	RenderManager();
	virtual ~RenderManager();

	void init(GLWidget* context);
	void render(double elapsed_time, SceneGraph* sg);
};

#endif // RENDERMANAGER_H
