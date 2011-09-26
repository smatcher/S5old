#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <core/graphics/framebufferobject.h>
#include <core/graphics/viewpoint.h>

class RenderTarget {

public:
	RenderTarget(Viewpoint* viewpoint, FrameBufferObject* buffer, int height, int width, bool on_screen) : m_viewpoint(viewpoint), m_buffer(buffer), m_height(height), m_width(width), m_on_screen(on_screen) {}
	virtual ~RenderTarget() {}

	Viewpoint* getViewpoint() {return m_viewpoint;}
	void bindAsTarget() {m_buffer->bindAsTarget();}
	void releaseAsTarget() {m_buffer->releaseAsTarget();}

	int getHeight() const {return m_height;}
	int getWidth() const {return m_width;}
	bool isOnScreen() const {return m_on_screen;}

private:
	Viewpoint* m_viewpoint;
	FrameBufferObject* m_buffer;
	int m_height;
	int m_width;
	bool m_on_screen;
};

#endif // RENDERTARGET_H
