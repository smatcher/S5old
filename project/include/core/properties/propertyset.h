#ifndef PROPERTYSET_H
#define PROPERTYSET_H

#include "iproperty.h"

class IProperty;

class PropertySet : public ParentOf<IProperty>
{
public:
    PropertySet();
};

#endif // PROPERTYSET_H
