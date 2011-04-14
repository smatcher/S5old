#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/resources/resource.h"
#include <QtOpenGL>

class TextureData : public ResourceData
{
protected:
	int m_height;
	int m_width;
	GLuint m_gltexture;
	bool m_hasgltex;

public:
	TextureData(const QString& name, const QString& path, IResourceFactory* factory) : ResourceData(name,path,factory), m_hasgltex(false) {}
	void bind()
	{
		if(m_hasgltex)
		{
			glBindTexture(GL_TEXTURE_2D , m_gltexture);
		}
	}
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
