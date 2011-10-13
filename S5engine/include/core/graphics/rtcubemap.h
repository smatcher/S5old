#ifndef RTCUBEMAP_H
#define RTCUBEMAP_H

#include <core/graphics/rendertexture.h>

#include <QtOpenGL>

static GLenum cubemap_targets[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

class RenderTextureCubemap : public RenderTexture
{
public:
	RenderTextureCubemap(QString name, int height, int width, GLenum format, GLenum type);
	virtual bool unload();

	virtual bool isCubemap() {return true;}

	virtual GLuint getRenderTextureId(int i= 0);
	virtual void swap();

	virtual void bind(int i=0)
	{
		if(m_hasgltex)
		{
			glEnable(GL_TEXTURE_CUBE_MAP);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_gltexture);
		}
	}

	virtual void release(int i=0)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_CUBE_MAP , 0);
	}

private:
	GLuint m_render_cube;
};

#endif // RTCUBEMAP_H
