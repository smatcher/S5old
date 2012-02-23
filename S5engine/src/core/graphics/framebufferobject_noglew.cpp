#include <core/graphics/framebufferobject.h>

#include <core/graphics/rendertexture.h>

void FrameBufferObject::attachTexture(RenderTexture* tex, AttachmentPoint attachment, GLenum textarget)
{
	bool valid = false;

	if(tex != NULL) {
		valid = tex->getWidth() == m_width && tex->getHeight() == m_height;
	}

	if(!valid) {
		if(tex == NULL) {
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
	Attachment at = {tex,attachment,textarget};
	m_textures.push_back(at);
}

void FrameBufferObject::swapTextures()
{
	for(int i=0 ; i<m_textures.size() ; i++) {
		RenderTexture* tex = m_textures[i].tex;
		tex->swap();
	}
}

int FrameBufferObject::GLEW_HACK_getRTIdForPass(RenderTexture* tex, int passNb)
{
	return tex->getRenderTextureId(passNb);
}
