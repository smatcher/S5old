// Manageehis include should fail since parenting.h includes parenting.hpp
// However QtCreator needs this include in order to provide code assistance.
#include "manager.h"

template <class Managee>
Manager<Managee> *Manager<Managee>::instance = 0;

template <class Managee>
Manager<Managee>::Manager()
{
}

template <class Managee>
Manager<Managee>::~Manager()
{
}


template <class Managee>
Manager<Managee>* Manager<Managee>::getInstance()
{
    if(instance == 0)
        instance = new Manager<Managee>();
    return instance;
}

template <class Managee>
void Manager<Managee>::add(Managee* managee)
{
    registeredManagees += managee;
}

template <class Managee>
void Manager<Managee>::remove(Managee* managee)
{
    int index = registeredManagees.indexOf(managee);
    registeredManagees.remove(index);
}
