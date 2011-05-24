#ifndef STBIMAGE_H
#define STBIMAGE_H

#include "core/graphics/texture.h"
#include "core/resources/iresourcefactory.h"
#include "core/utils/stb_image.h"

#include <QtOpenGL>

class StbImageFactory;

class StbImage : public TextureData
{
	friend class StbImageFactory;

private:
	stbi_uc* m_data;
	int m_comp;
	bool m_mipmap;

public:
	StbImage(const QString& name, const QString& path, IResourceFactory* factory, stbi_uc* data = NULL, bool mipmap = true);
	virtual bool unload();

	void buildGLTexture();
};

class StbImageFactory : public IResourceFactory
{
public:
	virtual void load(ResourceData* resource);

protected:
	virtual void parseFile(const QString& path, QList<ResourceData*>& content, const QHash<QString,QString>& rules);
};

#endif // STBIMAGE_H
