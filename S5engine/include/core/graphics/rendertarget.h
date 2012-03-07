#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <core/graphics/framebufferobject.h>
#include <core/graphics/viewpoint.h>
#include <QList>
#include <QPair>

class RenderTarget {

public:

	RenderTarget(Viewpoint* viewpoint);
	RenderTarget(Viewpoint* viewpoint, FrameBufferObject* buffer, QList< QPair<RenderTexture*,IRD::FrameBuffer::Attachment> > rendertextures, bool on_screen, bool stretch_to_screen);
	RenderTarget(Viewpoint* viewpoint, FrameBufferObject* buffer, RenderTexture* rendertexture, IRD::FrameBuffer::Attachment attachmentpoint, bool on_screen, bool stretch_to_screen);
	virtual ~RenderTarget() {}

	Viewpoint* getViewpoint() {return m_viewpoint;}
	void bind();
	void release();
	int getNbPass();
	void setupPass(int passNb);
	void passDone();
	void setTextureMatrix(int passNb);

	int getHeight() const {return m_height;}
	int getWidth() const {return m_width;}
	bool isOnScreen() const {return m_on_screen;}
	bool isStretchedToScreen() const {return m_stretch_to_screen;}

private:
	Viewpoint* m_viewpoint;
	FrameBufferObject* m_buffer;
	QList<QPair<RenderTexture*,IRD::FrameBuffer::Attachment> > m_rendertextures;
	int m_height;
	int m_width;
	bool m_on_screen;
	bool m_stretch_to_screen;
};

#endif // RENDERTARGET_H
