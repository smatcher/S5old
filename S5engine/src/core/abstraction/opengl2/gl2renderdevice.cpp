#include <GL/glew.h>

#include "core/abstraction/opengl2/gl2renderdevice.h"

#include "core/log/log.h"

namespace IRD
{
	const GLuint textureTypes[] = {
		GL_TEXTURE_1D,
		GL_TEXTURE_2D,
		GL_TEXTURE_3D,
		GL_TEXTURE_CUBE_MAP
	};

	const GLuint textureInternalFormats[] = {
		GL_COMPRESSED_RGBA_S3TC_DXT1_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT3_EXT,
		GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
		GL_RGBA8,
		GL_RGBA16F,
		GL_RGBA32F,
		GL_DEPTH_COMPONENT24,
		GL_DEPTH24_STENCIL8_EXT
	};

	const GLuint textureFormats[] = {
		GL_RGBA,
		GL_RGBA,
		GL_RGBA,
		GL_RGBA,
		GL_RGBA,
		GL_RGBA,
		GL_DEPTH_COMPONENT,
		GL_DEPTH_STENCIL_EXT
	};

	const GLuint textureInputTypes[] = {
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_UNSIGNED_BYTE,
		GL_FLOAT,
		GL_FLOAT,
		GL_FLOAT,
		GL_UNSIGNED_INT_24_8_EXT
	};

	const GLuint framebufferAttachment[] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7,
		GL_DEPTH_ATTACHMENT,
		GL_STENCIL_ATTACHMENT,
		GL_DEPTH_STENCIL_ATTACHMENT
	};

	//Global
	int GL2RenderDevice::init()
	{
		glewInit();
	}

	bool GL2RenderDevice::isSupported(Feature feature)
	{
		if(feature == FEATURE_CORE)
			return true;
		else
			return false;
	}

	//Textures
	bool GL2Texture::bind(int texunit)
	{
		glActiveTexture(GL_TEXTURE0 + texunit);
		glBindTexture(textureTypes[m_params.m_type], m_texture_id);
	}

	void GL2Texture::unbind(int texunit)
	{
		glActiveTexture(GL_TEXTURE0 + texunit);
		glBindTexture(textureTypes[m_params.m_type], 0);
	}

	void GL2Texture::resize(int width, int height)
	{
		m_params.m_width = width;
		m_params.m_height = height;
		glTexImage2D(
					textureTypes[m_params.m_type],
					0,
					textureInternalFormats[m_params.m_format],
					m_params.m_width,
					m_params.m_height,
					0,
					textureFormats[m_params.m_format],
					textureInputTypes[m_params.m_format],
					0
				);
	}

	GL2Texture::GL2Texture(Params params, GLuint texture_id): Texture(params), m_texture_id(texture_id){}

	Texture* GL2RenderDevice::createTexture(Texture::Params params)
	{
		GLuint texture_id;
		glEnable(textureTypes[params.m_type]);
		glGenTextures(1,&texture_id);
		Texture* texture = new GL2Texture(params, texture_id);

		texture->bind(0);

		//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);//Useless

		if(params.m_samplerState & Texture::TSS_FILTER)
		{
			if(params.m_mipmapLevels > 0) {
				glTexParameteri(textureTypes[params.m_type], GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
				glTexParameteri(textureTypes[params.m_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			} else {
				glTexParameteri(textureTypes[params.m_type], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
				glTexParameteri(textureTypes[params.m_type], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			}
		}
		else
		{
			glTexParameteri(textureTypes[params.m_type], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(textureTypes[params.m_type], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		}

		if(params.m_samplerState & Texture::TSS_WRAP_U)
			glTexParameteri(textureTypes[params.m_type],GL_TEXTURE_WRAP_S, GL_REPEAT);
		else
			glTexParameteri(textureTypes[params.m_type],GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

		if(params.m_samplerState & Texture::TSS_WRAP_V)
			glTexParameteri(textureTypes[params.m_type],GL_TEXTURE_WRAP_T, GL_REPEAT);
		else
			glTexParameteri(textureTypes[params.m_type],GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		if(params.m_type == Texture::TT_TEX3D)
		{
			if(params.m_samplerState & Texture::TSS_WRAP_W)
				glTexParameteri(textureTypes[params.m_type],GL_TEXTURE_WRAP_R, GL_REPEAT);
			else
				glTexParameteri(textureTypes[params.m_type],GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		}

		if(params.m_genmipmap)
			glTexParameteri(textureTypes[params.m_type],GL_GENERATE_MIPMAP, GL_TRUE);
		else
			glTexParameteri(textureTypes[params.m_type],GL_GENERATE_MIPMAP, GL_FALSE);

		//debugGL("creating texture");

		return texture;
	}

	void GL2RenderDevice::destroyTexture(Texture* texture)
	{
		GL2Texture* gl2tex = static_cast<GL2Texture*>(texture);
		glDeleteTextures(1,&gl2tex->m_texture_id);
	}

	void GL2RenderDevice::sendTextureData(Texture* texture, int layer, int mipLevel, const void* data)
	{
		const Texture::Params& params = texture->getParams();
		//glTexSubImage2D(textureTypes[params.m_type], mipLevel, 0, 0, params.m_width, params.m_height, textureFormats[params.m_format], textureInputTypes[params.m_format], data);
		glTexImage2D(
					textureTypes[params.m_type],
					mipLevel,
					textureInternalFormats[params.m_format],
					params.m_width,
					params.m_height,
					0,
					textureFormats[params.m_format],
					textureInputTypes[params.m_format],
					data
				);

		//debugGL("sending texture data");
	}

	bool GL2RenderDevice::getTextureData(Texture* texture, int layer, int mipLevel, void* data){}

	//VertexBuffers
	GL2VertexBuffer::GL2VertexBuffer(int size): VertexBuffer(size) {}

	VertexBuffer* GL2RenderDevice::createVertexBuffer(int size){}
	void GL2RenderDevice::destroyVertexBuffer(VertexBuffer* vbo){}
	void GL2RenderDevice::sendVertexBufferData(VertexBuffer* vbo, int offset, int size, const void* data){}
	bool GL2RenderDevice::getVertexBufferData(VertexBuffer* vbo, void* data){}

	// Shaders
	void GL2Shader::bind(){}
	void GL2Shader::unbind(){}
	GL2Shader::GL2Shader():Shader(){}

	Shader* GL2RenderDevice::createShader(const char* vertexSrc, const char* fragmentsrc){}
	void GL2RenderDevice::destroyShader(Shader* shader){}

	//FrameBuffers
	void GL2FrameBuffer::bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer_id);
	}

	void GL2FrameBuffer::unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	GL2FrameBuffer::GL2FrameBuffer(Params params, GLuint framebuffer_id):FrameBuffer(params),m_framebuffer_id(framebuffer_id){}

	FrameBuffer* GL2RenderDevice::createFrameBuffer(FrameBuffer::Params params)
	{
		GLuint framebuffer_id;
		glGenFramebuffers(1,&framebuffer_id);

		FrameBuffer* framebuffer = new GL2FrameBuffer(params, framebuffer_id);

		return framebuffer;
	}

	void GL2RenderDevice::destroyFrameBuffer(FrameBuffer* buffer)
	{
		GL2FrameBuffer* framebuffer = static_cast<GL2FrameBuffer*>(buffer);
		glDeleteFramebuffers(1,&framebuffer->m_framebuffer_id);
	}

	void GL2RenderDevice::attachTextureToFrameBuffer(FrameBuffer* buffer, Texture* texture, FrameBuffer::Attachment attachment)
	{
		GL2Texture* gl2texture = static_cast<GL2Texture*>(texture);
		buffer->bind();
		glFramebufferTexture2D(GL_FRAMEBUFFER, framebufferAttachment[attachment], textureTypes[texture->getType()], gl2texture->m_texture_id,0);
	}

	void GL2RenderDevice::checkFrameBuffer()
	{
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

	void GL2RenderDevice::resizeFrameBuffer(FrameBuffer* buffer, int width, int height)
	{
		//FrameBuffer::Params& params = buffer->
	}


	void GL2RenderDevice::setViewport(const Viewport& viewport)
	{
		glViewport(viewport.x, viewport.y, viewport.width, viewport.height);
	}

}
