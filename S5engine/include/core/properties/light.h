#ifndef LIGHT_H
#define LIGHT_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/lightingmanager.h"

class Light : public IProperty, public Managee<LightingManager>
{
public:
	Light();
	void sendParameters(int lightid);
	virtual void drawDebug(const GLWidget* widget, const RenderManager::DebugGizmosFilter& filter) const;
};

#endif // LIGHT_H
