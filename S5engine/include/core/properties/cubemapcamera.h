#ifndef CUBEMAPCAMERA_H
#define CUBEMAPCAMERA_H

#include "core/properties/iproperty.h"
#include "core/graphics/viewpoint.h"
#include "core/graphics/rendertexture.h"
#include "core/graphics/texture.h"

class CubemapCamera : public IProperty, public Viewpoint
{
public :
	/// Constructror
	CubemapCamera(double znear, double zfar);
	/// Destructor
	virtual ~CubemapCamera();

	virtual int getNbProjections() {return 6;}
	virtual Viewpoint::Style getStyle() {return CUBEMAP;}

	//virtual const Matrix4d& getProjection(double aspect, int projection_nb);
	virtual void setProjection(double aspect, double scale, int projection_nb);

	void setParameters(double znear, double zfar);

	virtual void applyTransform(int projection_nb);
	virtual void applyOnlyRotation(int projection_nb);

	void createTarget(int height, int width);
	Texture getTargetTexture();

private :
	double m_znear;
	double m_zfar;

	RenderTexture* m_render_texture;

	Matrix4d m_projection;

	void computeProjection();
};

#endif // CUBEMAPCAMERA_H
