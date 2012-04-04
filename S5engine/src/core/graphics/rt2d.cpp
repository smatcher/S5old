#include <GL/glew.h>
#include <GL/gl.h>

#include "core/graphics/rt2d.h"
#include "core/resources/managers.h"
#include "core/managers/rendermanager.h"

RenderTexture2D::RenderTexture2D(QString name, int height, int width, IRD::Texture::Format format)
	: RenderTexture(name, height, width)
{
	IRD::Texture::Params params;
	m_state = STATE_LOADED;

	params.m_type = IRD::Texture::TT_TEX2D;
	params.m_format = format;
	params.m_height = height;
	params.m_width = width;
	params.m_mipmapLevels = 0;
	params.m_genmipmap = false;
	params.m_samplerState = IRD::Texture::TSS_FILTER;

	m_irdtexture = RENDER_MANAGER.getRenderDevice()->createTexture(params);
	RENDER_MANAGER.getRenderDevice()->sendTextureData(m_irdtexture,0,0,0);
	m_back_texture = RENDER_MANAGER.getRenderDevice()->createTexture(params);
	RENDER_MANAGER.getRenderDevice()->sendTextureData(m_back_texture,0,0,0);

	TEXTURE_MANAGER.add(this);
}

RenderTexture2D::~RenderTexture2D()
{
	RENDER_MANAGER.getRenderDevice()->destroyTexture(m_irdtexture);
	RENDER_MANAGER.getRenderDevice()->destroyTexture(m_back_texture);
	m_irdtexture = 0;
	m_back_texture = 0;
	TEXTURE_MANAGER.remove(this);
}

void RenderTexture2D::resize(int height, int width)
{
	m_height = height;
	m_width = width;

	m_back_texture->bind(0);
	m_back_texture->resize(width, height);

	m_irdtexture->bind(0);
	m_irdtexture->resize(width, height);

/*
	glBindTexture(GL_TEXTURE_2D, m_gltexture);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, m_type, 0);

	glBindTexture(GL_TEXTURE_2D, m_render_texture);
	glTexImage2D(GL_TEXTURE_2D, 0, m_format, width, height, 0, m_format, m_type, 0);
*/

#ifdef WITH_TOOLS
	if(m_widget)
	{
		m_widget->updateData();
	}
#endif
}

bool RenderTexture2D::unload()
{
	return false;
}

void RenderTexture2D::bind(int i)
{
	for(int j=0 ; j< m_texture_matrices.size() ; j++) {
		RENDER_MANAGER.setTextureMatrix(QMatrix4x4(m_texture_matrices[j].values),j);
	}
	m_irdtexture->bind(i);
}

IRD::Texture* RenderTexture2D::getBackTexture(int i)
{
	return m_back_texture;
}

void RenderTexture2D::swap()
{
	IRD::Texture* tmp = m_irdtexture;
	m_irdtexture = m_back_texture;
	m_back_texture = tmp;
}

void RenderTexture2D::setTextureMatrix(const Matrix4d& texture_matrix, int i)
{
	while(m_texture_matrices.size() <= i) {
		m_texture_matrices.push_back(Matrix4d());
	}

	m_texture_matrices[i] = texture_matrix;
}
