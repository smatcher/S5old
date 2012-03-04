#include <GL/glew.h>

#include "core/abstraction/opengl2/gl2renderdevice.h"

namespace IRD
{
	const GLuint textureTypes[] = {GL_TEXTURE_1D, GL_TEXTURE_2D, GL_TEXTURE_3D, GL_TEXTURE_CUBE_MAP};

	//Global
	int init(){}
	bool isSupported(){return true;}

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

	GL2Texture::GL2Texture(Params params): Texture(params){}

	Texture GL2RenderDevice::createTexture(Texture::Params params){}
	void GL2RenderDevice::destroyTexture(Texture& texture){}
	void GL2RenderDevice::sendTextureData(Texture& texture, int layer, int mipLevel, int offset, int size, const void* data){}
	bool GL2RenderDevice::getTextureData(Texture& texture, int layer, int mipLevel, void* data){}

	//VertexBuffers
	GL2VertexBuffer::GL2VertexBuffer(int size): VertexBuffer(size) {}

	VertexBuffer GL2RenderDevice::createVertexBuffer(int size){}
	void GL2RenderDevice::destroyVertexBuffer(VertexBuffer vbo){}
	void GL2RenderDevice::sendVertexBufferData(VertexBuffer& vbo, int offset, int size, const void* data){}
	bool GL2RenderDevice::getVertexBufferData(VertexBuffer& vbo, void* data){}

	// Shaders
	void GL2Shader::bind(){}
	void GL2Shader::unbind(){}
	GL2Shader::GL2Shader():Shader(){}

	Shader GL2RenderDevice::createShader(const char* vertexSrc, const char* fragmentsrc){}
	void GL2RenderDevice::destroyShader(Shader& shader){}

	//FrameBuffers
	void GL2FrameBuffer::bind(){}
	GL2FrameBuffer::GL2FrameBuffer(Params params):FrameBuffer(params){}

	FrameBuffer GL2RenderDevice::createFrameBuffer(FrameBuffer::Params params){}
	void GL2RenderDevice::destroyFrameBuffer(FrameBuffer& buffer){}
}
