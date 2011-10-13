#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "core/graphics/texture.h"

#include <QtOpenGL>

class RenderTexture : public TextureData
{
public:
	RenderTexture(QString name, int height, int width);
	virtual bool unload() = 0;

	virtual GLuint getRenderTextureId(int i= 0) = 0;
	virtual void swap() = 0;
};

#endif // RENDERTEXTURE_H
