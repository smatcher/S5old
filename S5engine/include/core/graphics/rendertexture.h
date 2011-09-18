#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "core/graphics/texture.h"

#include <QtOpenGL>

class Camera;

class RenderTexture : public TextureData
{
public:

	RenderTexture(QString name, int height, int width, Camera* camera);
	virtual bool unload();

	void bindAsTarget();
	void releaseAsTarget();

	Camera* getCamera();

private:
	QGLFramebufferObject m_buffer;
	Camera* m_camera;
};

#endif // RENDERTEXTURE_H
