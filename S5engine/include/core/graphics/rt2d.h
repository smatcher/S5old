#ifndef RT2D_H
#define RT2D_H

#include "core/graphics/rendertexture.h"

class RenderTexture2D : public RenderTexture
{
public:
	RenderTexture2D(QString name, int height, int width, GLenum format, GLenum type);
	~RenderTexture2D();

	virtual void resize(int height, int width);

	virtual bool unload();

	virtual bool isCubemap() {return false;}

	virtual GLuint getRenderTextureId(int i= 0);
	virtual void swap();

	virtual void setTextureMatrix(const Matrix4d& texture_matrix, int i = 0) {}
private:
	GLuint m_render_texture;
	GLenum m_format;
	GLenum m_type;
};

#endif // RT2D_H
