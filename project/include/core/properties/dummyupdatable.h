#ifndef DUMMYUPDATABLE_H
#define DUMMYUPDATABLE_H

#include "include/core/properties/iupdatable.h"

class DummyUpdatable : public IUpdatable
{
public:
    DummyUpdatable();
    void update(double elapsed_time);

private:
    double angle;

};

#endif // DUMMYUPDATABLE_H
