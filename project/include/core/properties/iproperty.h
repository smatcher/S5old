#ifndef IPROPERTY_H
#define IPROPERTY_H

#include "include/core/utils/parenting.h"
#include "include/core/properties/propertyset.h"

class PropertySet;

class IProperty : public ChildOf<PropertySet>
{
public:
    /// Constructor
	IProperty(const QString& name = "Basic Property");
	~IProperty();
};

#endif // IPROPERTY_H
