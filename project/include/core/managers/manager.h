#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>

template <class Managee>
class Manager
{
public:
    Manager();
    ~Manager();

    static Manager<Managee>* getInstance();

    void add(Managee* managee);
    void remove(Managee* managee);

private:
    static Manager<Managee>* instance;

protected:
    QVector<Managee*> registeredManagees;
};

#include "manager.hpp"

#endif // MANAGER_H
