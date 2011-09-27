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

	void bindAsTarget();
	void releaseAsTarget();

	enum AttachmentPoint {
		COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT
	};

	void attachTexture(RenderTexture* tex, AttachmentPoint attachment);

private:
	GLuint m_framebuffer;
	GLuint m_renderbuffer;
	int m_width;
	int m_height;
	bool m_on_screen;

	QList< QPair<RenderTexture*, AttachmentPoint> > m_textures;
};

#endif // FRAMEBUFFEROBJECT_H
