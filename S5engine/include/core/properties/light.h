#ifndef LIGHT_H
#define LIGHT_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/lightingmanager.h"

#include "core/graphics/viewpoint.h"

class RenderTexture;

class Light : public IProperty, public Managee<LightingManager>, public Viewpoint
{
private:
	bool m_casts_shadows;
	RenderTexture* m_shadowmap;

public:
	Light(bool casts_shadows = false);
	void sendParameters(int lightid);
	virtual void drawDebug(const GLWidget* widget) const;

	virtual int getNbProjections();
	virtual Viewpoint::Style getStyle();

	virtual void setProjection(double aspect, double scale, int projection_nb);

	virtual void applyTransform(int projection_nb);
	virtual void applyOnlyRotation(int projection_nb);

	bool castsShadows();
	RenderTexture* getRenderTexture();
};

#endif // LIGHT_H
