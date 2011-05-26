// Manageehis include should fail since parenting.h includes parenting.hpp
// However QtCreator needs this include in order to provide code assistance.
#include "manager.h"

template <class Managee>
Manager<Managee>::Manager()
{
}

template <class Managee>
Manager<Managee>::~Manager()
{
}

template <class Managee>
const QVector<Managee*>& Manager<Managee>::managees() const
{
    return registeredManagees;
}

template <class Managee>
void Manager<Managee>::add(Managee* managee)
{
    registeredManagees += managee;
    onManageeAdded(managee);
}

template <class Managee>
void Manager<Managee>::remove(Managee* managee)
{
    int index = registeredManagees.indexOf(managee);
    registeredManagees.remove(index);
    onManageeRemoved(managee);
}

template <class Managee>
void Manager<Managee>::onManageeAdded(Managee*)
{
}

template <class Managee>
void Manager<Managee>::onManageeRemoved(Managee*)
{
}
