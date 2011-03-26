#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/properties/irenderable.h"

class Mesh;
class Material;

class MeshRenderer : public IRenderable
{
private:
	Mesh*     m_mesh;
	Material* m_material;
};

#endif // MESHRENDERER_H
