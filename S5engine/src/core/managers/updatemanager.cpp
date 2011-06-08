#include "core/managers/updatemanager.h"
#include "core/managers/physicsmanager.h"
#include "core/properties/iupdatable.h"
#include <iostream>

UpdateManager::UpdateManager()
{
}

UpdateManager::~UpdateManager()
{
}

void UpdateManager::update(double elapsed_time)
{
	PHYSICS_MANAGER.update(elapsed_time);

	//std::cout<< registeredManagees.count() << " Updatable nodes to update." << std::endl;

	for(int index = 0; index < registeredManagees.count(); index++)
	{
		IUpdatable* prop = (IUpdatable*)registeredManagees[index];
		prop->update(elapsed_time);
	}
}
