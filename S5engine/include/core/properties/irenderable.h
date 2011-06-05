#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "core/properties/iproperty.h"
#include "core/managers/managee.h"
#include "core/managers/rendermanager.h"

class RenderManager;

class IRenderable : public IProperty, public Managee<RenderManager>
{
public:
	/// Constructor
	IRenderable(const QString& name = "Basic Renderable");
	/// Destructor
	virtual ~IRenderable();

	virtual void render(double elapsed_time, GLWidget* context) = 0;
	virtual bool isTransparent() {return false;}
};

#endif // IRENDERABLE_H
