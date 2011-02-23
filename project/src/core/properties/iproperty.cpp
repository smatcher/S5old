#include "include/core/properties/iproperty.h"

IProperty::IProperty(QString propName = "Basic Property") : ChildOf<PropertySet>(propName)
{
}
