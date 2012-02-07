#ifndef RTCUBEMAP_H
#define RTCUBEMAP_H

#include <core/graphics/rendertexture.h>

#include <GL/gl.h>

class RenderTextureCubemap : public RenderTexture
{
public:
	RenderTextureCubemap(QString name, int height, int width, GLenum format, GLenum type);
	~RenderTextureCubemap();

	virtual void resize(int height, int width);

	virtual bool unload();

	virtual bool isCubemap() {return true;}

	virtual GLuint getRenderTextureId(int i= 0);
	virtual void swap();

	virtual void setTextureMatrix(const Matrix4d& texture_matrix, int i = 0) {}

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
	GLenum m_format;
	GLenum m_type;
};

#endif // RTCUBEMAP_H
