#include "managee.h"

template <class Manager>
Managee<Manager>::Managee()
{
    Manager* manager = (Manager*)Manager::getInstance();
    manager->add((typename Manager::ManagedType*)this);
}



template <class Manager>
Managee<Manager>::~Managee()
{
    Manager* manager = (Manager*)Manager::getInstance();
    manager->remove((typename Manager::ManagedType*)this);
}
