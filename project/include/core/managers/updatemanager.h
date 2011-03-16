#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "core/managers/manager.h"
#include "core/utils/singleton.h"

class IUpdatable;
class UpdateManager : public Manager<IUpdatable>
{
	friend class Singleton<UpdateManager>;

protected:
	UpdateManager();

public:
    virtual ~UpdateManager();

    void update(double elapsed_time);
};

typedef Singleton<UpdateManager> UPDATE_MANAGER;

#endif // UPDATEMANAGER_H
