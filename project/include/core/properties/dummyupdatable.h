#ifndef DUMMYUPDATABLE_H
#define DUMMYUPDATABLE_H

#include "include/core/properties/iupdatable.h"

class DummyUpdatable : IUpdatable
{
public:
    DummyUpdatable();
    void update(float elapsed_time);

private:
    float angle;

};

#endif // DUMMYUPDATABLE_H
