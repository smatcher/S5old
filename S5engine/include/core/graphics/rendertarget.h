#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <core/graphics/framebufferobject.h>
#include <core/graphics/viewpoint.h>

class RenderTarget {

public:
	RenderTarget(Viewpoint* viewpoint, FrameBufferObject* buffer, RenderTexture* rendertexture, bool on_screen);
	virtual ~RenderTarget() {}

	Viewpoint* getViewpoint() {return m_viewpoint;}
	void bind();
	void release();
	int getNbPass();
	void setupPass(int passNb);
	void passDone();

	int getHeight() const {return m_height;}
	int getWidth() const {return m_width;}
	bool isOnScreen() const {return m_on_screen;}

private:
	Viewpoint* m_viewpoint;
	FrameBufferObject* m_buffer;
	RenderTexture* m_rendertexture;
	int m_height;
	int m_width;
	bool m_on_screen;
};

#endif // RENDERTARGET_H
