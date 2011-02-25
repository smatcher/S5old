#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "include/core/managers/manager.h"
#include "include/core/properties/iupdatable.h"


class IUpdatable;
class UpdateManager : public Manager<IUpdatable>
{
public:
    UpdateManager();
    virtual ~UpdateManager();

    void update(double elapsed_time);
};

#endif // UPDATEMANAGER_H
