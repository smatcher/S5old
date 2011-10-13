#include <GL/glew.h>

#include <core/graphics/rtcubemap.h>
#include "core/resources/managers.h"

#include <QtOpenGL>

RenderTextureCubemap::RenderTextureCubemap(QString name, int height, int width, GLenum format, GLenum type)
	: RenderTexture(name, height, width)
{
	m_hasgltex = true;
	m_state = STATE_LOADED;

	debugGL("creating cubemap");

	glGenTextures(1, &m_gltexture);
	glGenTextures(1, &m_render_cube);
	glEnable(GL_TEXTURE_CUBE_MAP);

	debugGL("creating cubemap");

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_gltexture);

	debugGL("creating cubemap");

	for(int i=0 ; i<6 ; i++) {
		glTexImage2D(cubemap_targets[i], 0, format, width, height, 0, format, type, 0);
		debugGL("creating cubemap" << i);
	}


	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_FALSE);

	debugGL("creating cubemap");

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_render_cube);


	debugGL("creating cubemap");

	for(int i=0 ; i<6 ; i++) {
		glTexImage2D(cubemap_targets[i], 0, format, width, height, 0, format, type, 0);
		debugGL("creating cubemap" << i);
	}


	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_FALSE);

	debugGL("creating cubemap");

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	TEXTURE_MANAGER.add(this);

	debugGL("creating cubemap");
}

bool RenderTextureCubemap::unload()
{
	return false;
}

GLuint RenderTextureCubemap::getRenderTextureId(int i)
{
	return m_render_cube;
}

void RenderTextureCubemap::swap()
{
	GLuint tmp = m_gltexture;
	m_gltexture = m_render_cube;
	m_render_cube = tmp;
}
