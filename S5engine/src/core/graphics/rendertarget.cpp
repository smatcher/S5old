#include <GL/glew.h>

#include "core/graphics/rendertarget.h"
#include "core/graphics/rendertexture.h"
#include "core/managers/rendermanager.h"
#include "core/maths/vector2.h"

#include <QtOpenGL>

RenderTarget::RenderTarget(Viewpoint* viewpoint) :
	m_viewpoint(viewpoint),
	m_buffer(NULL),
	m_height(0),
	m_width(0),
	m_on_screen(true)
{
	Vector2i vp_size = RENDER_MANAGER.getCurrentViewportSize();
	m_height = vp_size.x;
	m_width = vp_size.y;
}

RenderTarget::RenderTarget(Viewpoint *viewpoint, FrameBufferObject *buffer, QList<QPair<RenderTexture*,FrameBufferObject::AttachmentPoint> > rendertextures, bool on_screen, bool stretch_to_screen) :
		m_viewpoint(viewpoint),
		m_buffer(buffer),
		m_rendertextures(rendertextures),
		m_height(0),
		m_width(0),
		m_on_screen(on_screen),
		m_stretch_to_screen(stretch_to_screen)
{
	if(m_buffer != NULL) {
		m_height = m_buffer->getHeight();
		m_width = m_buffer->getWidth();
	}
}

RenderTarget::RenderTarget(Viewpoint *viewpoint, FrameBufferObject *buffer, RenderTexture *rendertexture, FrameBufferObject::AttachmentPoint attachmentpoint, bool on_screen, bool stretch_to_screen) :
		m_viewpoint(viewpoint),
		m_buffer(buffer),
		m_height(0),
		m_width(0),
		m_on_screen(on_screen),
		m_stretch_to_screen(stretch_to_screen)
{
	if(m_buffer != NULL) {
		m_height = m_buffer->getHeight();
		m_width = m_buffer->getWidth();
	}
	if(rendertexture != NULL) {
		m_rendertextures.push_back(QPair<RenderTexture*, FrameBufferObject::AttachmentPoint>(rendertexture, attachmentpoint));
	}
}

int RenderTarget::getNbPass()
{
	return m_viewpoint->getNbProjections();
}

void RenderTarget::bind()
{
	if(m_buffer != NULL) {
		m_buffer->bind();
	}
}

void RenderTarget::release()
{
	if(m_buffer != NULL) {
		m_buffer->swapTextures();
		m_buffer->release();
	}
}

void RenderTarget::setupPass(int passNb)
{
	switch(m_viewpoint->getStyle()) {
		case Viewpoint::MONO:
			if(m_buffer != NULL) {
				for(int i = 0 ; i< m_rendertextures.size() ; i++) {
					m_buffer->attachTexture(m_rendertextures[i].first,
											m_rendertextures[i].second,
											GL_TEXTURE_2D);
				}
			}
			break;
		case Viewpoint::CUBEMAP:
			if(m_buffer != NULL) {
				GLenum target = 0;
				if(passNb == 0) {
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
				} else if(passNb == 1) {
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
				} else if(passNb == 2) {
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
				} else if(passNb == 3) {
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
				} else if(passNb == 4) {
					target = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
				} else if(passNb == 5) {
					target = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
				} else {
					logError("asked to render pass" << passNb << "on a cubemap");
				}

				if(target != 0) {
					for(int i = 0 ; i< m_rendertextures.size() ; i++) {
						m_buffer->attachTexture(m_rendertextures[i].first,
												m_rendertextures[i].second,
												target);
					}
				}
			}
			break;
		case Viewpoint::PROXY_CUBEMAP:
			if(m_buffer != NULL) {
				for(int i = 0 ; i< m_rendertextures.size() ; i++) {
					m_buffer->attachTexture(m_rendertextures[i].first,
											m_rendertextures[i].second,
											GL_TEXTURE_2D);
				}
			}
			break;
		default:
			logError("Unknown viewpoint style");
	}

	if(m_buffer != NULL) {
		m_buffer->commitTextures(passNb);
	}
}

void RenderTarget::passDone()
{

}

void RenderTarget::setTextureMatrix(int passNb)
{
	GLint mode;
	static double modelView[16];
	static double projection[16];
	static double final[16];
	const GLdouble biais[16] = {
		0.5, 0.0, 0.0, 0.0,
		0.0, 0.5, 0.0, 0.0,
		0.0, 0.0, 0.5, 0.0,
		0.5, 0.5, 0.5, 1.0
	};

	debugGL("1");
	glGetDoublev(GL_PROJECTION_MATRIX, projection);
	debugGL("2");
	glGetDoublev(GL_MODELVIEW_MATRIX, modelView);
	debugGL("3");
	glGetIntegerv(GL_MATRIX_MODE, &mode);
	debugGL("4");

	glMatrixMode(GL_TEXTURE);
	glPushMatrix();
	glLoadIdentity();
	glLoadMatrixd(biais);
	glMultMatrixd(projection);
	glMultMatrixd(modelView);
	glGetDoublev(GL_TEXTURE_MATRIX, final);
	glPopMatrix();
	glMatrixMode(mode);

	Matrix4d mat(final);

	switch(m_viewpoint->getStyle()) {
		case Viewpoint::PROXY_CUBEMAP :
			for(int i=0 ; i<m_rendertextures.size() ; i++) {
				m_rendertextures[i].first->setTextureMatrix(mat,passNb);
			}
			break;
		default:
			logWarn("Not implemented");
	}
}
