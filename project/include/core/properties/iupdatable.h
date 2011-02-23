#ifndef IUPDATABLE_H
#define IUPDATABLE_H

#include "include/core/properties/iproperty.h"

class IUpdatable : public IProperty
{
public:
    /// Constructor
	IUpdatable(const QString& name = "Basic Updatable");
	/// Destructor
	~IUpdatable();

	virtual void update(float elapsed_time) = 0;
};

#endif // IUPDATABLE_H
