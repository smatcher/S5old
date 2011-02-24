#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>

template <class T>
class Manager
{
public:
    Manager();
    ~Manager();

    static Manager<T>* getInstance();

    void add(T* managee);
    void remove(T* managee);

private:
    static Manager<T>* instance;

    QVector<T*> registeredManagees;
};

#include "manager.hpp"

#endif // MANAGER_H
