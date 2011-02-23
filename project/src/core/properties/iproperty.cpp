#include "include/core/properties/iproperty.h"

IProperty::IProperty(QString propName = QString("Basic Property")) : ChildOf<PropertySet>(propName)
{
}
