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

	void makeCurrent();
	Camera* getCamera();

private:
	QGLPixelBuffer m_buffer;
	Camera* m_camera;
};

#endif // RENDERTEXTURE_H
