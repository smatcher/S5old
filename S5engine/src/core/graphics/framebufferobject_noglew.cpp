#include <core/graphics/framebufferobject.h>

#include <core/graphics/rendertexture.h>

void FrameBufferObject::attachTexture(RenderTexture* tex, IRD::FrameBuffer::Attachment attachment)
{
	bool valid = false;

	if(tex != 0) {
		valid = tex->getWidth() == m_width && tex->getHeight() == m_height;
	}

	if(!valid) {
		if(tex == 0) {
			logError("Could not attach texture to FBO texture is not valid");
		} else {
			logError("Could not attach texture to FBO, dimensions don't match");
		}
		return;
	}

	for(int i=0 ; i< m_textures.count() ; i++) {
		if(m_textures[i].ap == attachment) {
			m_textures.removeAt(i);
		}
	}
	Attachment at = {tex,attachment};
	m_textures.push_back(at);
}

void FrameBufferObject::swapTextures()
{
	for(int i=0 ; i<m_textures.size() ; i++) {
		RenderTexture* tex = m_textures[i].tex;
		tex->swap();
	}
}

IRD::Texture* FrameBufferObject::GLEW_HACK_getTextureforPass(RenderTexture* tex, int passNb)
{
	return tex->getBackTexture(passNb);
}
