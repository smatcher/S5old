#ifndef RTARRAY_H
#define RTARRAY_H

#include <core/graphics/rendertexture.h>

#include <QtOpenGL>

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
	virtual bool unload();

	virtual bool isCubemap() {return false;}

	virtual GLuint getRenderTextureId(int i= 0);
	virtual void swap();

	virtual void bind(int i=0)
	{
		if(m_hasgltex)
		{
			for(int j=0 ; j< m_depth ; j++) {
				glActiveTexture(GL_TEXTURE0 + i + j);
				glBindTexture(GL_TEXTURE_2D, m_gltextures[i+j]);
			}
		}
	}

	virtual void release(int i=0)
	{
		for(int j=0 ; j< m_depth ; j++) {
			glActiveTexture(GL_TEXTURE0 + i + j);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}

private:
	GLuint* m_render_textures;
	GLuint* m_gltextures;
	int m_depth;
};

#endif // RTARRAY_H
