#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/properties/irenderable.h"
#include "core/graphics/mesh.h"
#include "core/graphics/material.h"

class MeshRenderer : public IRenderable
{
public:
	MeshRenderer(Mesh& mesh, Material& material);
	void render(double elapsed_time, GLWidget* context);

private:
	Mesh     m_mesh;
	Material m_material;
};

#endif // MESHRENDERER_H
