#include "core/graphics/rendertexture.h"

RenderTexture::RenderTexture(QString name, int height, int width, Camera *camera) : TextureData(name, "none", NULL), m_buffer(height, width)
{
	m_height = height;
	m_width = width;
	m_gltexture = m_buffer.generateDynamicTexture();
	m_hasgltex = true;

	m_camera = camera;
}

bool RenderTexture::unload()
{
	return false;
}

void RenderTexture::makeCurrent()
{
	m_buffer.makeCurrent();
}

Camera* RenderTexture::getCamera()
{
	return m_camera;
}
