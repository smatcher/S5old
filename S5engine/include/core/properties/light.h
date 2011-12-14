#ifndef LIGHT_H
#define LIGHT_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/lightingmanager.h"

#include "core/graphics/viewpoint.h"

#include "core/maths/vector4.h"

class RenderTexture;

class Light : public IProperty, public Managee<LightingManager>, public Viewpoint
{
public:

	enum Type
	{
		OMNI,
		SPOT,
		SUN
	};

private:
	bool m_casts_shadows;
	RenderTexture* m_shadowmap;

	float m_constant_attenuation;
	float m_linear_attenuation;
	float m_quadratic_attenuation;
	Vector4f m_diffuse_color;
	Vector4f m_specular_color;

	Type m_type;

public:
	Light(bool casts_shadows = false);
	void sendParameters(int lightid);

	virtual int getNbProjections();
	virtual Viewpoint::Style getStyle();

	virtual void setProjection(double aspect, double scale, int projection_nb);

	virtual void applyTransform(int projection_nb);
	virtual void applyOnlyRotation(int projection_nb);

	void setDiffuseColor(Vector4f color);
	void setSpecularColor(Vector4f color);
	void setAttenuation(float constant, float linear, float quadratic);
	void getAttenuation(float& constant, float& linear, float& quadratic);

	void setType(Type type);
	Type getType();

	bool castsShadows();
	RenderTexture* getRenderTexture();

	virtual void drawDebug(const GLWidget* widget, const RenderManager::DebugGizmosFilter& filter) const;
};

#endif // LIGHT_H
