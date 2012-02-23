#include <GL/glew.h>
#include <GL/gl.h>

#include <core/log/log.h>

#include <core/graphics/framebufferobject.h>

FrameBufferObject::FrameBufferObject(int height, int width, bool onscreen, bool add_depth_stencil_renderbuffer)
{
	m_height = height;
	m_width = width;
	m_on_screen = onscreen;

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
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1,&m_framebuffer);

	if(m_hasrenderbuffer) {
		glDeleteRenderbuffers(1,&m_renderbuffer);
	}
}

void FrameBufferObject::resize(int height, int width)
{
	debugGL("before resize");
	m_height = height;
	m_width = width;

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
}

void FrameBufferObject::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void FrameBufferObject::release()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
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
		AttachmentPoint attachment = m_textures[i].ap;
		if(attachment != DEPTH_ATTACHMENT && attachment != STENCIL_ATTACHMENT) {
			nb_color_buffers++;
		}
		glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, m_textures[i].textarget, GLEW_HACK_getRTIdForPass(tex,passNb),0);
		debugGL("attaching");
	}

	// Specify draw and read buffers
	if(nb_color_buffers > 0) {
		int i=0;
		GLenum* bufs = new GLenum[nb_color_buffers]();
		for(int j=0 ; j<m_textures.size() ; j++) {
			AttachmentPoint attachment = m_textures[j].ap;
			if(attachment != DEPTH_ATTACHMENT && attachment != STENCIL_ATTACHMENT) {
				bufs[i] = attachment;
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

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER_EXT);
	if(status != GL_FRAMEBUFFER_COMPLETE_EXT) {
		if(status == GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT) {
			logError("FBO status incomplete");
		//} else if(status == GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS) {
		//	logError("FBO status incomplete dimensions");
		} else if(status == GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT) {
			logError("FBO status incomplete missing attachement");
		} else if(status == GL_FRAMEBUFFER_UNSUPPORTED) {
			logError("FBO status unsupported");
		} else {
			logError("FBO status other");
		}
	}
}
