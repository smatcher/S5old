#include "core/properties/irenderable.h"

IRenderable::IRenderable(const QString& propName) : IProperty(propName), Managee<RenderManager>()
{
}

IRenderable::~IRenderable()
{

}
