#include "managee.h"
#include "core/utils/singleton.h"

template <class Manager>
Managee<Manager>::Managee()
{
	Manager* manager = &(Singleton<Manager>::getInstance());
	manager->add((typename Manager::ManagedType*)this);
}



template <class Manager>
Managee<Manager>::~Managee()
{
	Manager* manager = &(Singleton<Manager>::getInstance());
    manager->remove((typename Manager::ManagedType*)this);
}
