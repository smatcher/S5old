#ifndef CAMERA_H
#define CAMERA_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/cameramanager.h"
#include "core/maths/matrix4.h"
#include "core/graphics/rendertexture.h"
#include "core/graphics/viewpoint.h"

class Camera : public IProperty, public Managee<CameraManager>, public Viewpoint
{
public :
	/// Constructror
	Camera(double yfov, double znear, double zfar);
	/// Destructor
	virtual ~Camera();

	virtual int getNbProjections() {return 1;}
	virtual Viewpoint::Style getStyle() {return MONO;}

	virtual Vector3f getWorldPosition();
	virtual Frustum getFrustum();

	virtual const Matrix4d& getProjection(double aspect, int projection_nb);
	virtual void setProjection(double aspect, double scale, int projection_nb);

	void setParameters(double yfov, double znear, double zfar);

	virtual void applyTransform(int projection_nb);
	virtual void applyOnlyRotation(int projection_nb);

	void createTarget(int height, int width);
	Texture getTargetTexture();

private :
	bool m_needComputation;
	double m_yfov;
	double m_znear;
	double m_zfar;
	double m_lastAspect;

	RenderTexture* m_render_texture;

	Matrix4d m_projection;

	void computeProjection();

	/// Debug
	virtual void drawDebug(const GLWidget*, const RenderManager::DebugGizmosFilter& filter) const;

	#ifdef WITH_TOOLS
		virtual void onLinked(PropertySet *);
		virtual void onUnlinked(PropertySet *);
	#endif
};

#endif // CAMERA_H
