#include "include/core/properties/iproperty.h"

IProperty::IProperty(const QString& propName) : ChildOf<PropertySet>(propName)
{
}

IProperty::~IProperty()
{

}
