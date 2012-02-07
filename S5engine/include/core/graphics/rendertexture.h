#ifndef RENDERTEXTURE_H
#define RENDERTEXTURE_H

#include "core/graphics/texture.h"
#include "core/maths/matrix4.h"

#include <GL/gl.h>

class RenderTexture : public TextureData
{
public:
	RenderTexture(QString name, int height, int width);
	virtual bool unload() = 0;

	virtual void resize(int height, int width) = 0;

	virtual GLuint getRenderTextureId(int i= 0) = 0;
	virtual void swap() = 0;

	virtual void setTextureMatrix(const Matrix4d& texture_matrix, int i = 0) = 0;
};

#endif // RENDERTEXTURE_H
