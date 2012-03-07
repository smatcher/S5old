#include <GL/glew.h>
#include <GL/gl.h>

#include <core/graphics/rtcubemap.h>
#include "core/resources/managers.h"


static GLenum cubemap_targets[] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

RenderTextureCubemap::RenderTextureCubemap(QString name, int height, int width, GLenum format, GLenum type)
	: RenderTexture(name, height, width)
{
	/*
	m_hasgltex = true;
	m_state = STATE_LOADED;
	m_format = format;
	m_type = type;

	glGenTextures(1, &m_gltexture);
	glGenTextures(1, &m_render_cube);
	glEnable(GL_TEXTURE_CUBE_MAP);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_gltexture);

	for(int i=0 ; i<6 ; i++) {
		glTexImage2D(cubemap_targets[i], 0, format, width, height, 0, format, type, 0);
	}


	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_render_cube);

	for(int i=0 ; i<6 ; i++) {
		glTexImage2D(cubemap_targets[i], 0, format, width, height, 0, format, type, 0);
	}

	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameterf(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_GENERATE_MIPMAP, GL_FALSE);

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	TEXTURE_MANAGER.add(this);
	*/
}

RenderTextureCubemap::~RenderTextureCubemap()
{
	/*
	glDeleteTextures(1, &m_render_cube);
	glDeleteTextures(1, &m_gltexture);
	TEXTURE_MANAGER.remove(this);
	*/
}

void RenderTextureCubemap::resize(int height, int width)
{
	/*
	m_height = height;
	m_width = width;

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_gltexture);
	for(int i=0 ; i<6 ; i++) {
		glTexImage2D(cubemap_targets[i], 0, m_format, width, height, 0, m_format, m_type, 0);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_render_cube);
	for(int i=0 ; i<6 ; i++) {
		glTexImage2D(cubemap_targets[i], 0, m_format, width, height, 0, m_format, m_type, 0);
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	*/
}

bool RenderTextureCubemap::unload()
{
	return false;
}

IRD::Texture* RenderTextureCubemap::getBackTexture(int i)
{
	return 0;//m_render_cube;
}

void RenderTextureCubemap::swap()
{
	/*
	GLuint tmp = m_gltexture;
	m_gltexture = m_render_cube;
	m_render_cube = tmp;
	*/
}

