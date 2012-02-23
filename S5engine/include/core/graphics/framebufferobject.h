#ifndef FRAMEBUFFEROBJECT_H
#define FRAMEBUFFEROBJECT_H

#ifdef _WIN32
	#define GL_COLOR_ATTACHMENT0 0x8CE0
	#define GL_COLOR_ATTACHMENT1 0x8CE1
	#define GL_COLOR_ATTACHMENT2 0x8CE2
	#define GL_COLOR_ATTACHMENT3 0x8CE3
	#define GL_COLOR_ATTACHMENT4 0x8CE4
	#define GL_COLOR_ATTACHMENT5 0x8CE5
	#define GL_COLOR_ATTACHMENT6 0x8CE6
	#define GL_COLOR_ATTACHMENT7 0x8CE7
	#define GL_DEPTH_ATTACHMENT 0x8D00
	#define GL_STENCIL_ATTACHMENT 0x8D20
#endif

#include <GL/gl.h>
#include <QList>

//#include <core/graphics/rendertexture.h>
class RenderTexture;

class FrameBufferObject
{
public:
	FrameBufferObject(int height, int width, bool onscreen, bool add_depth_stencil_renderbuffer);
	virtual ~FrameBufferObject();

	int getHeight() { return m_height; }
	int getWidth() { return m_width; }

	void resize(int height, int width);

	void bind();
	void release();

	enum AttachmentPoint {
		COLOR_ATTACHMENT_0 = GL_COLOR_ATTACHMENT0,
		COLOR_ATTACHMENT_1 = GL_COLOR_ATTACHMENT1,
		COLOR_ATTACHMENT_2 = GL_COLOR_ATTACHMENT2,
		COLOR_ATTACHMENT_3 = GL_COLOR_ATTACHMENT3,
		COLOR_ATTACHMENT_4 = GL_COLOR_ATTACHMENT4,
		COLOR_ATTACHMENT_5 = GL_COLOR_ATTACHMENT5,
		COLOR_ATTACHMENT_6 = GL_COLOR_ATTACHMENT6,
		COLOR_ATTACHMENT_7 = GL_COLOR_ATTACHMENT7,
		DEPTH_ATTACHMENT = GL_DEPTH_ATTACHMENT,
		STENCIL_ATTACHMENT = GL_STENCIL_ATTACHMENT
	};

	void attachTexture(RenderTexture* tex, AttachmentPoint attachment, GLenum textarget = GL_TEXTURE_2D);
	void clearAttachments();
	void commitTextures(int passNb);
	void swapTextures();

private:
	GLuint m_framebuffer;
	GLuint m_renderbuffer;
	bool m_hasrenderbuffer;
	int m_width;
	int m_height;
	bool m_on_screen;

	struct Attachment {
		RenderTexture* tex;
		AttachmentPoint ap;
		GLenum textarget;
	};
	QList<Attachment> m_textures;

	int GLEW_HACK_getRTIdForPass(RenderTexture* tex, int passNb);
};

#endif // FRAMEBUFFEROBJECT_H
