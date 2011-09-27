#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "core/graphics/texture.h"

#include <QtOpenGL>

class RenderTexture : public TextureData
{
public:
	RenderTexture(QString name, int height, int width, GLenum format, GLenum type);
	virtual bool unload();

	GLuint getRenderTextureId();
	void swap();

private:
	GLuint m_render_texture;
};

#endif // RENDERTEXTURE_H
