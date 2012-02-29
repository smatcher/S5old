#ifndef IRENDERDEVICE_H
#define IRENDERDEVICE_H

namespace IRD
{
	class Texture
	{
	public:

		enum Type
		{
			TEX1D,
			TEX2D,
			TEX2DARRAY,
			TEX3D,
			CUBEMAP
		};

		enum Format
		{
			DXT1,
			DXT3,
			DXT5,
			BGRA8,
			RGBA16F,
			RGBA32F,
			DEPTH,
			DEPTH_STENCIL
		};

		struct Params
		{
			Type m_type;
			Format m_format;
			int m_width;
			int m_height;
			int m_nbLayers;
			bool m_genmipmap;
			bool m_mipmap;
			bool m_filtering;
			bool m_wrap_s;
			bool m_wrap_t;

			Params():
				m_type(TEX2D),
				m_format(BGRA8),
				m_width(256),
				m_height(256),
				m_nbLayers(1),
				m_genmipmap(true),
				m_mipmap(true),
				m_filtering(true),
				m_wrap_s(true),
				m_wrap_t(true)
			{}
		};

		virtual bool bind(int texunit) {return false;}
		virtual void unbind(int texunit) {return false;}

		Type   getType() {return m_params.m_type;}
		Format getFormat() {return m_params.m_format;}
		int    getWidth() {return m_params.m_width;}
		int    getHeight() {return m_params.m_height;}
		int    getNbLayers() {return m_params.m_nbLayers;}

	private:

		Params m_params;
	};

	class VertexBuffer
	{
	public:

		
	};

	class FrameBuffer
	{
	public:

	};

	class iRenderDevice
	{
	public:
		//Global
		virtual int init() {}
		virtual bool isSupported() = 0;

		//Textures
		IRD::Texture createTexture(Texture::Params params);

		//VertexBuffer

		//FrameBuffer
	};
}

#endif //IRENDERDEVICE_H
