#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#include <core/graphics/texture.h>
#include <QtOpenGL>

class FrameBufferObject
{
public:
	FrameBufferObject(int width, int height, bool onscreen);
	virtual ~FrameBufferObject();

	void bindAsTarget();
	void releaseAsTarget();

	enum AttachmentPoint {
		COLOR_ATTACHMENT = GL_COLOR_ATTACHMENT0,
		DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT
	};

	void attachTexture(Texture tex, AttachmentPoint attachment);

private:
	GLuint m_buffer;
	int m_width;
	int m_height;
	bool m_on_screen;
};

#endif // FRAMEBUFFEROBJECT_H
