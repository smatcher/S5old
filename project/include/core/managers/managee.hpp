#include "managee.h"

template <class Manager>
Managee<Manager>::Managee()
{
    Manager::getInstance()->add(this);
}


template <class Manager>
Managee<Manager>::~Managee()
{
    Manager::getInstance()->remove(this);
}
