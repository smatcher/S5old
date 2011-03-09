#ifndef MANAGEE_H
#define MANAGEE_H

#include "core/managers/manager.h"

template <class Manager>
class Managee
{
public:
    Managee();
    virtual ~Managee();
};

#include "managee.hpp"

#endif // MANAGEE_H
