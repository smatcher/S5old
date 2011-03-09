#include "core/properties/iupdatable.h"

IUpdatable::IUpdatable(const QString& propName) : IProperty(propName), Managee<UpdateManager>()
{
}

IUpdatable::~IUpdatable()
{

}
