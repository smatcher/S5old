#ifndef GL2RENDERDEVICE_H
#define GL2RENDERDEVICE_H

#include "core/abstraction/interface/irenderdevice.h"

#include <GL/gl.h>

namespace IRD
{
	class GL2RenderDevice;

	class GL2Texture : public Texture
	{
		friend class GL2RenderDevice;

	public:

		virtual bool bind(int texunit);
		virtual void unbind(int texunit);
		virtual void resize(int width, int height);

	protected:

		GL2Texture(Params params, GLuint texture_id);

		GLuint m_texture_id;
	};

	class GL2VertexBuffer : public VertexBuffer
	{
		friend class GL2RenderDevice;

	public:

	protected:

		GL2VertexBuffer(int size);
	};

	class GL2Shader : public Shader
	{
		friend class GL2RenderDevice;

	public:
		virtual void bind();
		virtual void unbind();

	protected:

		GL2Shader();
	};

	class GL2FrameBuffer : public FrameBuffer
	{
		friend class GL2RenderDevice;

	public:

		virtual void bind();
		virtual void unbind();

	protected:

		GL2FrameBuffer(Params params, GLuint framebuffer_id);

		GLuint m_framebuffer_id;
	};

	class GL2RenderDevice : public iRenderDevice
	{
	public:
		//Global
		virtual int init();
		virtual bool isSupported(Feature feature);

		//Textures
		virtual Texture* createTexture(Texture::Params params);
		virtual void destroyTexture(Texture* texture);
		virtual void sendTextureData(Texture* texture, int layer, int mipLevel, const void* data);
		virtual bool getTextureData(Texture* texture, int layer, int mipLevel, void* data);

		//VertexBuffers
		virtual VertexBuffer* createVertexBuffer(int size);
		virtual void destroyVertexBuffer(VertexBuffer* vbo);
		virtual void sendVertexBufferData(VertexBuffer* vbo, int offset, int size, const void* data);
		virtual bool getVertexBufferData(VertexBuffer* vbo, void* data);

		// Shaders
		virtual Shader* createShader(const char* vertexSrc, const char* fragmentsrc);
		virtual void destroyShader(Shader* shader);

		//FrameBuffers
		virtual FrameBuffer* createFrameBuffer(FrameBuffer::Params params);
		virtual void destroyFrameBuffer(FrameBuffer* buffer);
		virtual void attachTextureToFrameBuffer(FrameBuffer* buffer, Texture* texture, FrameBuffer::Attachment attachment);
		virtual void checkFrameBuffer();
		virtual void resizeFrameBuffer(FrameBuffer* buffer, int width, int height);

		//Misc
		virtual void setViewport(const Viewport& viewport);
	};
}

#endif // GL2RENDERDEVICE_H
