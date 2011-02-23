#ifndef IUPDATABLE_H
#define IUPDATABLE_H

#include "include/core/properties/iproperty.h"

class IUpdatable : public IProperty
{
public:
    /// Constructor
    IUpdatable(QString);

    virtual void update(float elapsed_time);
};

#endif // IUPDATABLE_H
