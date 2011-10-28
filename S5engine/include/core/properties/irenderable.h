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

	virtual void frameBegin(double elapsed_time) {}
	virtual void render() = 0;
	virtual void frameEnd() {}

	virtual bool isTransparent() {return false;}
	virtual bool receivesShadows() {return false;}
	virtual bool castsShadows() {return false;}
};

#endif // IRENDERABLE_H
