#include <GL/glew.h>

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
	} else {
		m_renderbuffer = -1;
	}
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1,&m_framebuffer);

	if(m_renderbuffer >= 0) {
		glDeleteRenderbuffers(1,&m_renderbuffer);
	}
}

void FrameBufferObject::bindAsTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
}

void FrameBufferObject::releaseAsTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBufferObject::attachTexture(Texture tex, AttachmentPoint attachment)
{
	bool valid = false;

	if(tex.isValid()) {
		valid = tex->getWidth() == m_width && tex->getHeight() == m_height;
	}

	if(!valid) {
		if(tex.isValid()) {
			logError("Could not attach texture to FBO, dimensions don't match");
		} else {
			logError("Could not attach texture to FBO texture is not valid");
		}
		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

	// attach the texture to FBO depth attachment point
	glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, tex->getGLId(),0);

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

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
