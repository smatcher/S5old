#ifndef IRENDERDEVICE_H
#define IRENDERDEVICE_H

namespace IRD
{
	class iRenderDevice;

	class Texture
	{

	public:

		enum Type
		{
			TT_TEX1D,
			TT_TEX2D,
			TT_TEX3D,
			TT_CUBEMAP
		};

		enum Format
		{
			TF_DXT1,
			TF_DXT3,
			TF_DXT5,
			TF_RGBA8,
			TF_RGBA16F,
			TF_RGBA32F,
			TF_DEPTH,
			TF_DEPTH_STENCIL
		};

		enum SamplerState
		{
			// Clamp wrap
			TSS_WRAP_U = 1<<0,
			TSS_WRAP_V = 1<<1,
			TSS_WRAP_W = 1<<2,
			TSS_WRAP   = TSS_WRAP_U | TSS_WRAP_V | TSS_WRAP_W,
			// Filter
			TSS_FILTER = 1<<3
		};

		struct Params
		{
			Type m_type;
			Format m_format;
			int m_width;
			int m_height;
			int m_nbLayers;
			bool m_genmipmap;
			int m_mipmapLevels;
			int m_samplerState;

			Params():
				m_type(TT_TEX2D),
				m_format(TF_RGBA8),
				m_width(256),
				m_height(256),
				m_nbLayers(1),
				m_genmipmap(true),
				m_mipmapLevels(7),
				m_samplerState(TSS_WRAP | TSS_FILTER)
			{}
		};

		virtual bool bind(int texunit) {}
		virtual void unbind(int texunit) {}

		Type   getType() {return m_params.m_type;}
		Format getFormat() {return m_params.m_format;}
		int    getWidth() {return m_params.m_width;}
		int    getHeight() {return m_params.m_height;}
		int    getNbLayers() {return m_params.m_nbLayers;}

		virtual void resize(int width, int height){}

		const Params& getParams() {return m_params;}

	protected:

		Texture(Params params):m_params(params){}
		Params m_params;
	};

	class VertexBuffer
	{
	public:
		int getSize() {return m_size;}

	protected:

		VertexBuffer(int size):m_size(size){}
		int m_size;
	};

	class Shader
	{
	public:
		virtual void bind(){}
		virtual void unbind(){}

	protected:

		Shader(){}
	};

	class FrameBuffer
	{
	public:

		enum Attachment
		{
			COLOR_ATTACHMENT_0,
			COLOR_ATTACHMENT_1,
			COLOR_ATTACHMENT_2,
			COLOR_ATTACHMENT_3,
			COLOR_ATTACHMENT_4,
			COLOR_ATTACHMENT_5,
			COLOR_ATTACHMENT_6,
			COLOR_ATTACHMENT_7,
			DEPTH_ATTACHMENT,
			STENCIL_ATTACHMENT,
			DEPTH_STENCIL_ATTACHMENT
		};

		struct Params
		{
			Texture::Format m_format;
			int m_width;
			int m_height;
			int m_nbColorBuffers;

			Params():
				m_format(Texture::TF_RGBA8),
				m_width(256),
				m_height(256),
				m_nbColorBuffers(1)
			{}
		};

		virtual void bind(){}
		virtual void unbind(){}

		const Params& getParams() {return m_params;}

	protected:

		FrameBuffer(Params params):m_params(params){}
		Params m_params;
	};

	class iRenderDevice
	{
	public:

		enum Feature
		{
			FEATURE_CORE
		};

		//Global
		virtual int init() = 0;
		virtual bool isSupported(Feature feature) {return false;}

		//Textures
		virtual Texture* createTexture(Texture::Params params) {return 0;}
		virtual void destroyTexture(Texture* texture) {}
		virtual void sendTextureData(Texture* texture, int layer, int mipLevel, const void* data) {}
		virtual bool getTextureData(Texture* texture, int layer, int mipLevel, void* data) {return false;}

		//VertexBuffers
		virtual VertexBuffer* createVertexBuffer(int size) {return 0;}
		virtual void destroyVertexBuffer(VertexBuffer* vbo) {}
		virtual void sendVertexBufferData(VertexBuffer* vbo, int offset, int size, const void* data) {}
		virtual bool getVertexBufferData(VertexBuffer* vbo, void* data) {return false;}

		// Shaders
		virtual Shader* createShader(const char* vertexSrc, const char* fragmentsrc) {return 0;}
		virtual void destroyShader(Shader* shader) {}

		//FrameBuffers
		virtual FrameBuffer* createFrameBuffer(FrameBuffer::Params params) {return 0;}
		virtual void destroyFrameBuffer(FrameBuffer* buffer) {}
		virtual void attachTextureToFrameBuffer(FrameBuffer* buffer, Texture* texture, FrameBuffer::Attachment attachment){}
		virtual void checkFrameBuffer(){}
		virtual void resizeFrameBuffer(FrameBuffer* buffer, int width, int height){}
	};
}

#endif //IRENDERDEVICE_H
