#ifndef MANAGER_H
#define MANAGER_H

#include <QVector>

template <class Managee>
class Manager
{
public:
    Manager();
	virtual ~Manager();

    static Manager<Managee>* getInstance();

    void add(Managee* managee);
    void remove(Managee* managee);

    typedef Managee ManagedType;

private:
    static Manager<Managee>* instance;

protected:
    QVector<Managee*> registeredManagees;
};

#include "manager.hpp"

#endif // MANAGER_H
