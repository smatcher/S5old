#ifndef RT2D_H
#define RT2D_H

#include "core/graphics/rendertexture.h"

class RenderTexture2D : public RenderTexture
{
public:
	RenderTexture2D(QString name, int height, int width, GLenum format, GLenum type);
	virtual bool unload();

	virtual bool isCubemap() {return false;}

	virtual GLuint getRenderTextureId(int i= 0);
	virtual void swap();

	virtual void setTextureMatrix(const Matrix4d& texture_matrix, int i = 0) {}
private:
	GLuint m_render_texture;
};

#endif // RT2D_H
