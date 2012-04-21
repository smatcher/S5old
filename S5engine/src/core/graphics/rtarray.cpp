/*
#include <GL/glew.h>
#include <GL/gl.h>
*/

#include <QtOpenGL>
#include <GL/glu.h>

#include <core/graphics/rtarray.h>
#include "core/resources/managers.h"
#include "core/managers/rendermanager.h"

#ifdef _WIN32
	#define GL_GENERATE_MIPMAP 0x8191
#endif

void __ClientActiveTexture(int texid);

RenderTextureArray::RenderTextureArray(QString name, int height, int width, int nbLayers, IRD::Texture::Format format)
	: RenderTexture(name, height, width), m_nbLayers(nbLayers)
{
	debugGL("before RenderTextureArray constructor");

	IRD::iRenderDevice* rd = RENDER_MANAGER.getRenderDevice();

	//m_hasgltex = true;
	m_state = STATE_LOADED;

	m_front_textures = new IRD::Texture*[m_nbLayers]();
	m_back_textures = new IRD::Texture*[m_nbLayers]();
	IRD::Texture::Params params;
	params.m_format = format;
	params.m_genmipmap = false;
	params.m_nbLayers = 0;
	params.m_mipmapLevels = 0;
	params.m_height = m_height;
	params.m_width = m_width;
	params.m_samplerState = IRD::Texture::TSS_FILTER;
	for(int i=0 ; i< m_nbLayers ; i++) {
		m_front_textures[i] = rd->createTexture(params);
		rd->sendTextureData(m_front_textures[i],0,0,0);
		m_back_textures[i] = rd->createTexture(params);
		rd->sendTextureData(m_back_textures[i],0,0,0);
	}

	TEXTURE_MANAGER.add(this);

	debugGL("RenderTextureArray constructor end");
}

RenderTextureArray::~RenderTextureArray()
{
	IRD::iRenderDevice* rd = RENDER_MANAGER.getRenderDevice();
	for(int i = 0 ; i< m_nbLayers ; i++)
	{
		rd->destroyTexture(m_front_textures[i]);
		rd->destroyTexture(m_back_textures[i]);
	}

	delete[] m_front_textures;
	delete[] m_back_textures;

	TEXTURE_MANAGER.remove(this);
}

void RenderTextureArray::resize(int height, int width)
{
	m_height = height;
	m_width = width;

	for(int i=0 ; i<m_nbLayers ; i++) {
		m_front_textures[i]->bind(0);
		m_front_textures[i]->resize(width, height);

		m_back_textures[i]->bind(0);
		m_back_textures[i]->resize(width, height);
	}
}

bool RenderTextureArray::unload()
{
	return false;
}

IRD::Texture* RenderTextureArray::getBackTexture(int i)
{
	if(i>= 0 && i< m_nbLayers)
		return m_back_textures[i];
	else
		return 0;
}

void RenderTextureArray::swap()
{
	for(int i=0 ; i< m_nbLayers ; i++) {
		IRD::Texture* tmp = m_front_textures[i];
		m_front_textures[i] = m_back_textures[i];
		m_back_textures[i] = tmp;
	}
}

void RenderTextureArray::bind(int i)
{
	for(int j=0 ; j< m_nbLayers ; j++) {
		/*
		if(m_texture_matrices.size() > j) {
			RENDER_MANAGER.setTextureMatrix(QMatrix4x4(m_texture_matrices[j].values),i,j);
		}
		*/
		m_front_textures[j]->bind(i + j);
	}
}

void RenderTextureArray::release(int i)
{
	for(int j=0 ; j< m_nbLayers ; j++) {
		__ClientActiveTexture(GL_TEXTURE0 + i + j);
		glMatrixMode(GL_TEXTURE);
		glLoadIdentity();
		glMatrixMode(GL_MODELVIEW);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_front_textures[j]->unbind(i + j);
	}
}

void RenderTextureArray::setTextureMatrix(const Matrix4d& texture_matrix, int i)
{
	while(m_texture_matrices.size() <= i) {
		m_texture_matrices.push_back(Matrix4d());
	}

	m_texture_matrices[i] = texture_matrix;
}

const Matrix4d& RenderTextureArray::getTextureMatrix(int i)
{
	return m_texture_matrices[i];
}
