#include "core/graphics/rendertarget.h"

RenderTarget::RenderTarget(Viewpoint *viewpoint, FrameBufferObject *buffer, RenderTexture *rendertexture, bool on_screen) :
		m_viewpoint(viewpoint),
		m_buffer(buffer),
		m_rendertexture(rendertexture),
		m_height(0),
		m_width(0),
		m_on_screen(on_screen)
{
	if(rendertexture != NULL) {
		m_height = rendertexture->getHeight();
		m_width = rendertexture->getWidth();
	}
}

int RenderTarget::getNbPass()
{
	return m_viewpoint->getNbProjections();
}

void RenderTarget::bind()
{
	if(m_buffer != NULL) {
		m_buffer->bind();
	}
}

void RenderTarget::release()
{
	if(m_buffer != NULL) {
		m_buffer->swapTextures();
		m_buffer->release();
	}
}

void RenderTarget::setupPass(int passNb)
{
	switch(m_viewpoint->getStyle()) {
		case Viewpoint::MONO:
			if(m_buffer != NULL) {
				m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_2D);
			}
			break;
		case Viewpoint::CUBEMAP:
			if(m_buffer != NULL) {
				if(passNb == 0) {
					m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
				} else if(passNb == 1) {
					m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
				} else if(passNb == 2) {
					m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
				} else if(passNb == 3) {
					m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
				} else if(passNb == 4) {
					m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
				} else if(passNb == 5) {
					m_buffer->attachTexture(m_rendertexture, FrameBufferObject::COLOR_ATTACHMENT, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
				} else {
					logError("asked to render pass" << passNb << "on a cubemap");
				}
			}
			break;
		default:
			logError("Unknown viewpoint style");
	}

	if(m_buffer != NULL) {
		m_buffer->commitTextures();
	}
}

void RenderTarget::passDone()
{

}
