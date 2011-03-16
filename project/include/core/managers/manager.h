#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>

template <class Managee>
class Manager
{
protected:
	Manager();
public:
	virtual ~Manager();

    void add(Managee* managee);
    void remove(Managee* managee);

    typedef Managee ManagedType;

protected:
    QVector<Managee*> registeredManagees;
};

#include "manager.hpp"

#endif // MANAGER_H
