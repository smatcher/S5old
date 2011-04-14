#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/properties/irenderable.h"
#include "core/graphics/mesh.h"
#include "core/graphics/material.h"
#include "core/graphics/texture.h"

class MeshRenderer : public IRenderable
{
public:
	MeshRenderer(Mesh& mesh, Material& material, Texture& texture);
	void render(double elapsed_time, GLWidget* context);

private:
	Mesh     m_mesh;
	Material m_material;
	Texture  m_texture;
};

#endif // MESHRENDERER_H
