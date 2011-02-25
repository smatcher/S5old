#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include "include/core/managers/manager.h"
#include "include/core/managers/managee.h"

class UpdateManager : public Manager< Managee<UpdateManager> >
{
public:
    UpdateManager();
    virtual ~UpdateManager();

    void update(double elapsed_time);
};

#endif // UPDATEMANAGER_H
