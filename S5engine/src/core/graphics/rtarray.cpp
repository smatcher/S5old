#include <GL/glew.h>

#include <core/graphics/rtarray.h>
#include "core/resources/managers.h"

#include <QtOpenGL>

RenderTextureArray::RenderTextureArray(QString name, int height, int width, int depth, GLenum format, GLenum type)
	: RenderTexture(name, height, width), m_depth(depth)
{
	m_hasgltex = true;
	m_state = STATE_LOADED;

	m_gltextures = new GLuint[m_depth]();
	m_render_textures = new GLuint[m_depth]();
	glGenTextures(m_depth, m_gltextures);
	glGenTextures(m_depth, m_render_textures);

	for(int i=0 ; i<m_depth ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_gltextures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, 0);
	}

	for(int i=0 ; i<m_depth ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_render_textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, 0);
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	TEXTURE_MANAGER.add(this);
}

bool RenderTextureArray::unload()
{
	return false;
}

GLuint RenderTextureArray::getRenderTextureId(int i)
{
	return m_render_textures[i];
}

void RenderTextureArray::swap()
{
	for(int i=0 ; i< m_depth ; i++) {
		GLuint tmp = m_gltextures[i];
		m_gltextures[i] = m_render_textures[i];
		m_render_textures[i] = tmp;
	}
}
