#ifndef TEXTURE_H
#define TEXTURE_H

#include "core/resources/resource.h"

class TextureData : public ResourceData
{
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
