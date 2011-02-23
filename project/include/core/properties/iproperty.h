#ifndef IPROPERTY_H
#define IPROPERTY_H

#include "include/core/utils/parenting.h"
#include "include/core/properties/propertyset.h"

class PropertySet;

class IProperty : public ChildOf<PropertySet>
{
public:
    /// Constructor
    IProperty(QString);
};

#endif // IPROPERTY_H
