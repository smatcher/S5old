#include <GL/glew.h>
#include <GL/gl.h>

#include "core/graphics/rendertexture.h"

RenderTexture::RenderTexture(QString name, int height, int width) : TextureData(name, "none", 0)
{
	m_height = height;
	m_width = width;
}

