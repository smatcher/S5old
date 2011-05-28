#ifndef LIGHTINGMANAGER_H
#define LIGHTINGMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class Light;

class LightingManager : public Manager<Light>
{
public :
	LightingManager();
	~LightingManager();
};

typedef Singleton<LightingManager> SingletonLightingManager;
#define LIGHTING_MANAGER SingletonLightingManager::getInstance()

#endif // LIGHTINGMANAGER_H
