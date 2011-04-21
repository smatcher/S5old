#ifndef MESHRENDERER_H
#define MESHRENDERER_H

#include "core/properties/irenderable.h"
#include "core/graphics/mesh.h"
#include "core/graphics/material.h"
#include "core/graphics/texture.h"

class MeshRendererWidget;

class MeshRenderer : public IRenderable
{
	friend class MeshRendererWidget;

public:
	MeshRenderer(Mesh& mesh, Material& material, Texture& texture);
	void render(double elapsed_time, GLWidget* context);

	virtual PropertyWidget* getWidget();
private:
	Mesh     m_mesh;
	Material m_material;
	Texture  m_texture;
};

#endif // MESHRENDERER_H
