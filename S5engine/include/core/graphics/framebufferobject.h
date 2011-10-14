#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include <core/graphics/rendertexture.h>
#include <QtOpenGL>
#include <QPair>

class FrameBufferObject
{
public:
	FrameBufferObject(int height, int width, bool onscreen, bool add_depth_stencil_renderbuffer);
	virtual ~FrameBufferObject();

	void bind();
	void release();

	enum AttachmentPoint {
		COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT
	};

	void attachTexture(RenderTexture* tex, AttachmentPoint attachment, GLenum textarget = GL_TEXTURE_2D);
	void commitTextures();
	void swapTextures();

private:
	GLuint m_framebuffer;
	GLuint m_renderbuffer;
	int m_width;
	int m_height;
	bool m_on_screen;

	struct Attachment {
		RenderTexture* tex;
		AttachmentPoint ap;
		GLenum textarget;
	};
	QList<Attachment> m_textures;
};

#endif // FRAMEBUFFEROBJECT_H
