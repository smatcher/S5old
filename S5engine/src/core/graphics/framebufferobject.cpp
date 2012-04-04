#include <GL/glew.h>
#include <GL/gl.h>

#include <core/log/log.h>

#include <core/graphics/framebufferobject.h>
#include "core/managers/rendermanager.h"

FrameBufferObject::FrameBufferObject(int height, int width, bool onscreen, bool add_depth_stencil_renderbuffer)
{
	m_height = height;
	m_width = width;
	m_on_screen = onscreen;

	IRD::FrameBuffer::Params params;
	params.m_height = height;
	params.m_width = width;

	m_framebuffer = RENDER_MANAGER.getRenderDevice()->createFrameBuffer(params);

	/*
	glGenFramebuffers(1,&m_framebuffer);

	if(add_depth_stencil_renderbuffer) {
		glGenRenderbuffers(1,&m_renderbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_renderbuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_renderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		m_hasrenderbuffer = true;
	} else {
		m_hasrenderbuffer = false;
	}
	*/
}

FrameBufferObject::~FrameBufferObject()
{
	if(m_framebuffer != 0)
		RENDER_MANAGER.getRenderDevice()->destroyFrameBuffer(m_framebuffer);

	m_framebuffer = 0;
	/*
	glDeleteFramebuffers(1,&m_framebuffer);

	if(m_hasrenderbuffer) {
		glDeleteRenderbuffers(1,&m_renderbuffer);
	}
	*/
}

void FrameBufferObject::resize(int height, int width)
{
	debugGL("before resize");
	m_height = height;
	m_width = width;

	/*
	if(m_hasrenderbuffer) {
		glBindRenderbuffer(GL_RENDERBUFFER_EXT, m_renderbuffer);
		glRenderbufferStorage(GL_RENDERBUFFER_EXT, GL_DEPTH_STENCIL, width, height);
		glBindRenderbuffer(GL_RENDERBUFFER_EXT, 0);

		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_renderbuffer);
		glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_STENCIL_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, m_renderbuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
	debugGL("resizing");
	*/
}

void FrameBufferObject::bind()
{
	if(m_framebuffer)
		m_framebuffer->bind();
}

void FrameBufferObject::release()
{
	if(m_framebuffer)
		m_framebuffer->unbind();
}

void FrameBufferObject::clearAttachments()
{
	m_textures.clear();
}

void FrameBufferObject::commitTextures(int passNb)
{
	int nb_color_buffers = 0;

	// attach the textures to FBO depth attachment point
	for(int i=0 ; i<m_textures.size() ; i++) {
		RenderTexture* tex = m_textures[i].tex;
		IRD::FrameBuffer::Attachment attachment = m_textures[i].ap;
		if(attachment != IRD::FrameBuffer::DEPTH_ATTACHMENT && attachment != IRD::FrameBuffer::STENCIL_ATTACHMENT && attachment != IRD::FrameBuffer::DEPTH_STENCIL_ATTACHMENT) {
			nb_color_buffers++;
		}
		RENDER_MANAGER.getRenderDevice()->attachTextureToFrameBuffer(m_framebuffer, GLEW_HACK_getTextureforPass(tex, passNb), attachment);
		debugGL("attaching");
	}

	// Specify draw and read buffers
	if(nb_color_buffers > 0) {
		int i=0;
		GLenum* bufs = new GLenum[nb_color_buffers]();
		for(int j=0 ; j<m_textures.size() ; j++) {
			IRD::FrameBuffer::Attachment attachment = m_textures[j].ap;
			if(attachment != IRD::FrameBuffer::DEPTH_ATTACHMENT && attachment != IRD::FrameBuffer::STENCIL_ATTACHMENT && attachment != IRD::FrameBuffer::DEPTH_STENCIL_ATTACHMENT) {
				bufs[i] = attachment + GL_COLOR_ATTACHMENT0;
				i++;
			}
		}
		glDrawBuffers(nb_color_buffers, bufs);
		//glReadBuffers(nb_color_buffers, bufs);
		delete[] bufs;
	} else {
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}

	RENDER_MANAGER.getRenderDevice()->checkFrameBuffer();

	debugGL("after commiting textures");
}
