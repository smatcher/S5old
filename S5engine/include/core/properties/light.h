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
};

#endif // LIGHT_H
