/*
#include <GL/glew.h>
#include <GL/gl.h>
*/

#include <QtOpenGL>
#include <GL/glu.h>

#include <core/graphics/rtarray.h>
#include "core/resources/managers.h"


RenderTextureArray::RenderTextureArray(QString name, int height, int width, int depth, GLenum format, GLenum type)
	: RenderTexture(name, height, width), m_depth(depth)
{
	debugGL("before RenderTextureArray constructor");

	m_hasgltex = true;
	m_state = STATE_LOADED;
	m_format = format;
	m_type = type;

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
		debugGL("RenderTextureArray constructor spec");
	}

	for(int i=0 ; i<m_depth ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_render_textures[i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, 0);
		debugGL("RenderTextureArray constructor spec");
	}

	glBindTexture(GL_TEXTURE_2D, 0);

	TEXTURE_MANAGER.add(this);

	debugGL("RenderTextureArray constructor end");
}

RenderTextureArray::~RenderTextureArray()
{
	glDeleteTextures(m_depth, m_gltextures);
	glDeleteTextures(m_depth, m_render_textures);
	TEXTURE_MANAGER.remove(this);
}

void RenderTextureArray::resize(int height, int width)
{
	m_height = height;
	m_width = width;

	for(int i=0 ; i<m_depth ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_gltextures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, m_type, 0);
	}

	for(int i=0 ; i<m_depth ; i++) {
		glBindTexture(GL_TEXTURE_2D, m_render_textures[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, m_type, 0);
	}
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

void RenderTextureArray::bind(int i)
{
	if(m_hasgltex)
	{
		for(int j=0 ; j< m_depth ; j++) {
			glActiveTexture(GL_TEXTURE0 + i + j);
			if(m_texture_matrices.size() > j) {
				glMatrixMode(GL_TEXTURE);
				m_texture_matrices[j].glLoadd();
				glMatrixMode(GL_MODELVIEW);
			}
			glBindTexture(GL_TEXTURE_2D, m_gltextures[j]);
		}
	}
}

void RenderTextureArray::release(int i)
{
	for(int j=0 ; j< m_depth ; j++) {
		glActiveTexture(GL_TEXTURE0 + i + j);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void RenderTextureArray::setTextureMatrix(const Matrix4d& texture_matrix, int i)
{
	while(m_texture_matrices.size() <= i) {
		m_texture_matrices.push_back(Matrix4d());
	}

	m_texture_matrices[i] = texture_matrix;
}

