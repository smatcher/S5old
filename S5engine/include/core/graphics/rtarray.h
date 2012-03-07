#ifndef RTARRAY_H
#define RTARRAY_H

#include <core/graphics/rendertexture.h>

#include <GL/gl.h>

/***

  Ok this class is ... special

  RenderTextureArray is an array of 2D RenderTextures (not a RenderTexture using GL_TEXTURE_2D_ARRAY)
  The reason behind this class is the lack of support for said GL_TEXTURE_2D_ARRAY on my netbook

  WARNING :

	whenever you bind this texture, it also binds to the n-1 texunits following i

  ***/

class RenderTextureArray : public RenderTexture
{
public:
	RenderTextureArray(QString name, int height, int width, int depth, GLenum format, GLenum type);
	~RenderTextureArray();

	virtual void resize(int height, int width);

	virtual bool unload();

	virtual bool isCubemap() {return false;}

	virtual IRD::Texture* getBackTexture(int i= 0);
	virtual void swap();

	virtual void bind(int i=0);
	virtual void release(int i=0);

	virtual void setTextureMatrix(const Matrix4d& texture_matrix, int i = 0);

private:
	GLuint* m_render_textures;
	GLuint* m_gltextures;
	QList<Matrix4d> m_texture_matrices;
	int m_depth;
	GLenum m_format;
	GLenum m_type;
};

#endif // RTARRAY_H
