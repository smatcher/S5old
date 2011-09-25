#ifndef RENDERTARGET_H
#define RENDERTARGET_H

#include <core/graphics/framebufferobject.h>
#include <core/properties/camera.h>

class RenderTarget {

public:
	RenderTarget(Camera* camera, FrameBufferObject* buffer) : m_camera(camera), m_buffer(buffer) {}
	virtual ~RenderTarget() {}

	Camera* getCamera() {return m_camera;}
	void bindAsTarget() {m_buffer->bindAsTarget();}
	void releaseAsTarget() {m_buffer->releaseAsTarget();}

private:
	Camera* m_camera;
	FrameBufferObject* m_buffer;
};

#endif // RENDERTARGET_H
