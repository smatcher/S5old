#ifndef TEXTUREWIDGET_H
#define TEXTUREWIDGET_H

#include "tools/widgets/resourcewidget.h"

class TextureData;

class TextureWidget : public ResourceWidget
{
public:
	TextureWidget(TextureData& resource);
	virtual ~TextureWidget();
};

#endif // TEXTUREWIDGET_H
