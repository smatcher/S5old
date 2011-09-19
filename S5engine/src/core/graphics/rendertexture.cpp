#include "core/graphics/rendertexture.h"
#include "core/resources/managers.h"

RenderTexture::RenderTexture(QString name, int height, int width, Camera *camera) : TextureData(name, "none", NULL), m_buffer(height, width, QGLFramebufferObject::CombinedDepthStencil)
{
	m_height = height;
	m_width = width;
	m_gltexture = m_buffer.texture();
	m_hasgltex = true;

	m_state = STATE_LOADED;

	m_camera = camera;

	TEXTURE_MANAGER.add(this);
}

bool RenderTexture::unload()
{
	return false;
}

void RenderTexture::bindAsTarget()
{
	m_buffer.bind();
}

void RenderTexture::releaseAsTarget()
{
	m_buffer.release();
}

Camera* RenderTexture::getCamera()
{
	return m_camera;
}
