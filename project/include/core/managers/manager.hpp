// This include should fail since parenting.h includes parenting.hpp
// However QtCreator needs this include in order to provide code assistance.
#include "manager.h"

template <class T>
Manager<T> *Manager<T>::instance = 0;

template <class T>
Manager<T>::Manager()
{
}

template <class T>
Manager<T>::~Manager()
{
}


template <class T>
Manager<T>::getInstance()
{
    if(this->instance == 0)
        this->instance = new Manager<T>();
    return this->instance;
}

template <class T>
void Manager<T>::add(T* managee)
{
    registeredManagees += managee;
}

template <class T>
void Manager<T>::remove(T* managee)
{
    int index = registeredManagees.indexOf(managee);
    registeredManagees.remove(index);
}
