#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <core/graphics/framebufferobject.h>
#include <core/properties/camera.h>

class RenderTarget {

public:
	RenderTarget(Camera* camera, FrameBufferObject* buffer, int height, int width, bool on_screen) : m_camera(camera), m_buffer(buffer), m_height(height), m_width(width), m_on_screen(on_screen) {}
	virtual ~RenderTarget() {}

	Camera* getCamera() {return m_camera;}
	void bindAsTarget() {m_buffer->bindAsTarget();}
	void releaseAsTarget() {m_buffer->releaseAsTarget();}

	int getHeight() {return m_height;}
	int getWidth() {return m_width;}
	bool isOnScreen() {return m_on_screen;}

private:
	Camera* m_camera;
	FrameBufferObject* m_buffer;
	int m_height;
	int m_width;
	bool m_on_screen;
};

#endif // RENDERTARGET_H
