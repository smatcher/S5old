#include <GL/glew.h>

#include "core/graphics/rendertexture.h"

#include <QtOpenGL>

RenderTexture::RenderTexture(QString name, int height, int width) : TextureData(name, "none", NULL)
{
	m_height = height;
	m_width = width;
}

