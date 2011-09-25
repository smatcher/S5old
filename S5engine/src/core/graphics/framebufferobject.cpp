#include <GL/glew.h>

#include <core/graphics/framebufferobject.h>

FrameBufferObject::FrameBufferObject(int height, int width, bool onscreen)
{
	m_height = height;
	m_width = width;
	m_on_screen = onscreen;

	glGenFramebuffers(1,&m_buffer);
}

FrameBufferObject::~FrameBufferObject()
{
	glDeleteFramebuffers(1,&m_buffer);
}

void FrameBufferObject::bindAsTarget()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);
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

	glBindFramebuffer(GL_FRAMEBUFFER, m_buffer);

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
