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

    const QVector<Managee*>& managees() const;

    typedef Managee ManagedType;

protected:
	QVector<Managee*> registeredManagees;

private:
	virtual void onManageeAdded(Managee* managee);
	virtual void onManageeRemoved(Managee* managee);

};

#include "manager.hpp"

#endif // MANAGER_H
