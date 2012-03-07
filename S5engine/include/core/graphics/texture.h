#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/resources/resource.h"
#include "core/abstraction/interface/irenderdevice.h"

#ifdef WITH_TOOLS
	#include "tools/widgets/texturewidget.h"
#endif

class Texture;
template <class R, class H> class ResourceManager;

class TextureData : public ResourceData
{
	friend class ResourceHandle<TextureData>;
	friend class ResourceManager<TextureData,Texture>;
#ifdef WITH_TOOLS
	friend class TextureWidget;
#endif

protected:
	int m_height;
	int m_width;
	IRD::Texture* m_irdtexture;

public:
	TextureData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory), m_irdtexture(0)
	{
	}

	virtual void bind(int i=0)
	{
		if(m_irdtexture != 0)
		{
			m_irdtexture->bind(i);
		}
	}

	virtual void release(int i=0)
	{
		if(m_irdtexture != 0)
		{
			m_irdtexture->unbind(i);
		}
	}

	virtual bool isCubemap() = 0;
	int getHeight() { return m_height; }
	int getWidth() { return m_width; }

#ifdef WITH_TOOLS
	virtual ResourceWidget* getWidget()
	{
		if(!m_widget)
			m_widget = new TextureWidget(*this);

		return m_widget;
	}
#endif
};

class Texture : public ResourceHandle<TextureData>
{
public:
	Texture() {}
	Texture(const Texture& from) : ResourceHandle<TextureData>(from) {}
	Texture(TextureData& from) : ResourceHandle<TextureData>(from) {}
	virtual ~Texture() {}
};

#endif // TEXTURE_H
